
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Renderer/External/GL/glcorearb.h"
#include "Engine/Renderer/External/GL/glext.h"
#include "Engine/Renderer/External/GL/wglext.h"
#pragma comment(lib, "opengl32")
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Vertex.hpp"
#include <vector>
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Renderer/RenderBuffer.hpp"
#include "Engine/Renderer/ShaderProgram.hpp"
#include "Engine/Renderer/Sampler.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/File/File.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/CommandDefinition.hpp"
#include "Engine/Console/DevConsole.hpp"
#include <ctime>
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Sprite.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Buffers/IndexBuffer.hpp"
#include "Engine/Renderer/Buffers/VertexBuffer.hpp"
#include "Engine/Renderer/DebugRenderer.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Buffers/UniformBuffer.hpp"
#include "Engine/Renderer/GIFAnimation.hpp"
#include "Engine/Renderer/Material.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty.hpp"
#include "Engine/Renderer/Lights/Light.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Renderer/TextureCube.hpp"
#include "Engine/Core/Clock.hpp"
//#include "Engine/Profiler/Profiler.hpp"

//-----------------------------------------------------------------------------------------------
// Rendering constants
const int defaultIndices[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

//-----------------------------------------------------------------------------------------------
// Context globals
void* g_displayDeviceContext = NULL;			// ...becomes WindowContext::m_displayContext
void* g_openGLRenderingContext = NULL;		// ...becomes RendererOpenGL::m_glRenderingContext
void* g_GLLibrary = NULL;

static Renderer* g_TheRenderer = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Renderer::Renderer()
{
	g_displayDeviceContext = ::GetDC((HWND) Window::GetInstance()->GetHandle());
}


//-----------------------------------------------------------------------------------------------
// Start of the frame 
//
void Renderer::BeginFrame()
{
	ResetDefaultMaterial();
	SetDefaultMaterial();
	SetTexture(m_defaultTexture);
}

//-----------------------------------------------------------------------------------------------
// End of a frame
//
void Renderer::EndFrame()
{
	// copies the default camera's framebuffer to the "null" framebuffer, 
	// also known as the back buffer.
	CopyFrameBuffer( nullptr, m_defaultCamera->m_frameBuffer ); 

	SwapBuffers(::GetDC((HWND) Window::GetInstance()->GetHandle()));
	ClearScreen(Rgba::BLACK);
}

//-----------------------------------------------------------------------------------------------
// Post startup operations on Renderer
//
void Renderer::PostStartup()
{
	// default_vao is a GLuint member variable
	glGenVertexArrays( 1, &m_defaultVAO ); 
	glBindVertexArray( m_defaultVAO ); 

	// Init built-in shaders (inline)
	ShaderProgram::InitializeBuiltInShaders();

	// Initialize the samplers
	Sampler::InitializeSamplers();

	// Generating the default textures
	Image::GenerateDefaultTextures();
	Image defaultPixel(*Image::GetDefaultTexture());
	m_defaultTexture = CreateOrGetTexture(defaultPixel);

	// Default color targets setup
	m_defaultColorTarget = CreateRenderTarget((unsigned int) Window::GetInstance()->m_width,(unsigned int) Window::GetInstance()->m_height, TEXTURE_FORMAT_RGBA8);
	m_defaultDepthTarget = CreateDepthStencilTarget((unsigned int) Window::GetInstance()->m_width,(unsigned int) Window::GetInstance()->m_height);
	
	// Default 2D camera setup
	m_defaultCamera = new Camera();
	m_defaultCamera->SetColorTarget(m_defaultColorTarget);
	m_defaultCamera->SetDepthTarget(m_defaultDepthTarget);

	// Default perspective camera setup
	m_defaultPerspectiveCam = new Camera();
	m_defaultPerspectiveCam->SetColorTarget(m_defaultColorTarget);
	m_defaultPerspectiveCam->SetDepthTarget(m_defaultDepthTarget);
	m_defaultPerspectiveCam->SetPerspective(60.f, 1.f, 0.1f, 100.f);

	// Generating engine meshes 
	InitializeDefaultMeshes();

	// Default shader setup
	m_defaultShader = new Shader(g_defDiffuseProgram);

	// Default material setup
	m_defaultMaterial = new Material(m_defaultShader);
	m_defaultMaterialShared = new Material(m_defaultShader);
	
	SetDefaultMaterial();

	SetCamera(nullptr);

	// Image effects setup
	m_effectCamera = new Camera();
	m_copySrc = new FrameBuffer();
	m_copyDestination = new FrameBuffer();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	m_lightBlock = new LightBlock();
	m_lightBuffer = new UniformBuffer();

	m_specularBlock = new SpecularBlock();
	m_specularBuffer = new UniformBuffer();

	m_cameraBuffer = UniformBuffer::For( CameraBlock() );
	
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	COMMAND("screenshot", ScreenshotCommand, "Takes a screenshot of the current frame");

	FogBlock fogParams = {};
	fogParams.FOG_COLOR = Rgba::WHITE.GetAsVector();
	fogParams.FOG_FAR_FACTOR = 0.75f;
	fogParams.FOG_NEAR_FACTOR = 0.001f;
	fogParams.FOG_FAR_PLANE = 20.f;
	fogParams.FOG_NEAR_PLANE = 18.f;
	m_fogBuffer = new UniformBuffer();
	m_fogBuffer->Set<FogBlock>(fogParams);
	m_fogBuffer->UpdateGPU();
	BindUBO(BLOCK_FOG, m_fogBuffer);

	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
// Returns the instance of the renderer
//
Renderer* Renderer::GetInstance()
{
	return g_TheRenderer;
}

//-----------------------------------------------------------------------------------------------
// Creates the instance of the renderer if it's not already created
//
Renderer* Renderer::CreateInstance()
{
	if(g_TheRenderer == nullptr)
	{
		g_TheRenderer = new Renderer();
	}
	
	return g_TheRenderer;
}

//-----------------------------------------------------------------------------------------------
// Deletes the instance of the renderer
//
void Renderer::DestroyInstance()
{
	delete g_TheRenderer;
	g_TheRenderer = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Command callback for taking a screenshot
//
bool Renderer::ScreenshotCommand(Command& cmd)
{
	char buffer[100];
	if(IsDevConsoleOpen())
	{
		DevConsole::GetInstance()->Close();
	}
	if(cmd.m_tokens.size() == 1)
	{
		//YYYYMMDD_HHMMSS
		time_t rawtime;
		struct tm * timeinfo;

		time (&rawtime);
		timeinfo = localtime (&rawtime);
		strftime(buffer, 80, "Screenshots\\screenshot_%Y%m%d_%H%M%S.png", timeinfo);
		g_TheRenderer->TakeScreenshot(buffer);
	}
	else
	{
		g_TheRenderer->TakeScreenshot(cmd.GetNextString().c_str());
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
// OpenGL Ortho matrix as the current projection matrix 
//
void Renderer::SetOrtho(const Vector2& bottomLeft, const Vector2& topRight, float nearPoint, float farPoint)
{
	SetOrtho(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, nearPoint, farPoint);	
}

//-----------------------------------------------------------------------------------------------
// OpenGL Ortho matrix as the current projection matrix 
//
void Renderer::SetOrtho(float left, float right, float down, float up, float zNear, float zFar)
{
	Matrix44 projectionMatrix = Matrix44::MakeOrtho3D(left, right, down, up, zNear, zFar);
	m_currentCamera->SetProjection(projectionMatrix);
}

//-----------------------------------------------------------------------------------------------
// OpenGL Ortho matrix as the current projection matrix 
//
void Renderer::SetOrtho(const Matrix44& projMatrix)
{
	m_currentCamera->SetProjection(projMatrix);
}

//-----------------------------------------------------------------------------------------------
// Sets ortho matrix from size, near and far
//
void Renderer::SetProjectionOrtho(float size, float zNear, float zFar)
{
	float horizontal = Window::GetInstance()->m_width * 0.5f * size;
	float vertical = Window::GetInstance()->m_height * 0.5f * size;
	Matrix44 projectionMatrix = Matrix44::MakeOrtho3D(-horizontal, horizontal, -vertical, vertical, zNear, zFar);
	m_currentCamera->SetProjection(projectionMatrix);
}

//-----------------------------------------------------------------------------------------------
// Sets the perspective matrix from the given parameters
//
void Renderer::SetPerspective(float fovDegrees, float aspect, float zNear, float zFar)
{
	Matrix44 projectionMatrix = Matrix44::MakePerspectiveMatrix(fovDegrees, aspect, zNear, zFar);
	m_currentCamera->SetProjection(projectionMatrix);
}

//-----------------------------------------------------------------------------------------------
// Sets the view matrix 
//
void Renderer::SetViewMatrix(const Matrix44& viewMatrix)
{
	m_currentCamera->m_viewMatrix = viewMatrix;
}

//-----------------------------------------------------------------------------------------------
// OpenGL Clear Screen with Color Wrapper
//
void Renderer::ClearScreen(const Rgba& clearColor)
{
 	glClearColor( clearColor.r, clearColor.g, clearColor.b, clearColor.a );
	GL_CHECK_ERROR();

 	glClear( GL_COLOR_BUFFER_BIT );
	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
// Clears the depth buffer
//
void Renderer::ClearDepth(float depth /*= 1.f */)
{
	glDepthMask(GL_TRUE);
	glClearDepthf( depth );
	GL_CHECK_ERROR();
	glClear( GL_DEPTH_BUFFER_BIT ); 
	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
// OpenGL Enable Blend
//
void Renderer::EnableBlend() const
{
	//glEnable(GL_BLEND);
}

//-----------------------------------------------------------------------------------------------
// OpenGL Enable line smooth
//
void Renderer::EnableLineSmooth() const
{
	glEnable(GL_LINE_SMOOTH);
}

//-----------------------------------------------------------------------------------------------
// Draw a line using 2D coordinates start and end, with line thickness and single color
//
void Renderer::DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& lineColor, float lineThickness)
{
	UNUSED(lineThickness);
	DrawLine2D( start, end, lineColor, lineColor, lineThickness );
}

//-----------------------------------------------------------------------------------------------
// Draw a line using 2D coordinates start and end, with line thickness and color on each vertex
//
void Renderer::DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& startColor, const Rgba& endColor, float lineThickness)
{
	UNUSED(lineThickness);
	Vertex_3DPCU vertices[2];
	vertices[0] = Vertex_3DPCU(Vector3(start), startColor, Vector2(0.f,0.f));
	vertices[1] = Vertex_3DPCU(Vector3(end), endColor, Vector2(0.f,0.f));
	
	DrawMeshImmediate(vertices, 2, PRIMITIVE_LINES);
}

//-----------------------------------------------------------------------------------------------
// Draws a line in 3D space
//
void Renderer::DrawLine3D(const Vector3& start, const Vector3& end, const Rgba& lineColor /*= Rgba::WHITE */)
{
	Vertex_3DPCU vertices[2];
	vertices[0] = Vertex_3DPCU(Vector3(start), lineColor, Vector2(0.f,0.f));
	vertices[1] = Vertex_3DPCU(Vector3(end), lineColor, Vector2(0.f,0.f));

	DrawMeshImmediate(vertices, 2, PRIMITIVE_LINES);
}

//-----------------------------------------------------------------------------------------------
// Draw a line loop using the set of vertices and line thickness with a single color
//
void Renderer::DrawLineLoop2D(const Vector2 vertices[], int numVertices, float lineThickness, const Rgba& objectColor)
{
	Rgba* vertexColors = new Rgba[numVertices];
	for(int index = 0; index < numVertices; ++index)
	{
		vertexColors[index] = objectColor; // Assigns same color to all vertices. (To cut down extra point of failure)
	}
	DrawLineLoop2D(vertices, numVertices, lineThickness, vertexColors);
	delete[] vertexColors;
}

//-----------------------------------------------------------------------------------------------
// Draw a line loop using the set of vertices and line thickness with a multi-colored vertices
//
void Renderer::DrawLineLoop2D(const Vector2 vertices[], int numVertices, float lineThickness, const Rgba vertexColors[])
{
	UNUSED(lineThickness);
	Vertex_3DPCU* vertexArray = new Vertex_3DPCU[numVertices];
	// Loop through vertex array and make the new vertex structures
	for(int index = 0; index < numVertices; index++)
	{
		vertexArray[index] = Vertex_3DPCU(Vector3(vertices[index]), vertexColors[index], Vector2(0.f,0.f));
	}
	DrawMeshImmediate(vertexArray, numVertices, PRIMITIVE_LINE_LOOP);
	delete[] vertexArray;
}

//-----------------------------------------------------------------------------------------------
// Draw a triangle using the set of vertices and line thickness with single color
//
void Renderer::DrawTriangle2D(const Vector2 vertices[], float lineThickness, const Rgba& objectColor)
{
	Rgba vertexColors[3];
	for(int index = 0; index < 3; ++index)
	{
		vertexColors[index] = objectColor; // Assigns same color to all vertices. (To cut down extra point of failure)
	}
	DrawTriangle2D(vertices, lineThickness, vertexColors);
}

//-----------------------------------------------------------------------------------------------
// Draw a triangle using the set of vertices and line thickness with multi-colored vertices
//
void Renderer::DrawTriangle2D(const Vector2 vertices[], float lineThickness, const Rgba vertexColors[])
{
	UNUSED(lineThickness);
	Vertex_3DPCU vertexArray[3]; // Triangle has 3 vertices
	// Loop through vertex array and make the new vertex structures
	for(int index = 0; index < 3; index++)
	{
		vertexArray[index] = Vertex_3DPCU(Vector3(vertices[index]), vertexColors[index], Vector2(0.f,0.f));
	}
	DrawMeshImmediate(vertexArray, 3, PRIMITIVE_TRIANGLES);
}

//-----------------------------------------------------------------------------------------------
// Draw a circle using the radius ,line thickness and color
//
void Renderer::DrawCircle2D(float radius, float lineThickness /*= 1.5f*/, const Rgba& objectColor /*= Rgba::WHITE */)
{
	UNUSED(lineThickness);
	Vertex_3DPCU vertices[NUM_VERTICES_FOR_CIRCLE];
	for (int i = 0; i<100; i++)
	{
		float startDegrees, endDegrees;
		Vector2 start, end;
		float angle = 3.6f;
		
		startDegrees = angle * static_cast<float>(i);
		endDegrees = angle * static_cast<float>(i+1);

		start.x = CosDegrees(startDegrees) * radius;
		start.y = SinDegrees(startDegrees) * radius;

		end.x = CosDegrees(endDegrees) * radius;
		end.y = SinDegrees(endDegrees) * radius;

		vertices[i] = Vertex_3DPCU(Vector3(start), objectColor, Vector2(0.f,0.f));
		vertices[i+1] = Vertex_3DPCU(Vector3(end),objectColor, Vector2(0.f,0.f));
	}
	DrawMeshImmediate(vertices, NUM_VERTICES_FOR_CIRCLE, PRIMITIVE_LINES);
}

//-----------------------------------------------------------------------------------------------
// Draw a circle using the radius ,line thickness and color
//
void Renderer::DrawDottedCircle2D(float radius, float lineThickness /*= 1.5f*/, const Rgba& objectColor /*= Rgba::WHITE*/)
{
	UNUSED(lineThickness);
	Vertex_3DPCU* vertices = new Vertex_3DPCU[NUM_VERTICES_FOR_CIRCLE];
	for (int i = 0; i<NUM_VERTICES_FOR_CIRCLE; i++)
	{
		float startDegrees, endDegrees;
		Vector2 start, end;
		float angle = 3.6f;
		if(i % 2 == 0)
		{
			startDegrees = angle * static_cast<float>(i);
			endDegrees = angle * static_cast<float>(i+1);

			start.x = CosDegrees(startDegrees) * radius;
			start.y = SinDegrees(startDegrees) * radius;

			end.x = CosDegrees(endDegrees) * radius;
			end.y = SinDegrees(endDegrees) * radius;

			vertices[i] = Vertex_3DPCU(Vector3(start), objectColor, Vector2(0.f,0.f));
			vertices[i+1] = Vertex_3DPCU(Vector3(end),objectColor, Vector2(0.f,0.f));
		}
	}
	DrawMeshImmediate(vertices, NUM_VERTICES_FOR_CIRCLE, PRIMITIVE_LINES);
	delete[] vertices;
}

//-----------------------------------------------------------------------------------------------
// Draws a quad on the screen with color
//
void Renderer::DrawAABB2(const AABB2& bounds, const Rgba& color)
{
	Vertex_3DPCU vertices[6];
	vertices[0] = Vertex_3DPCU(Vector3(bounds.mins.x, bounds.mins.y), color, Vector2(0.f,0.f));
	vertices[1] = Vertex_3DPCU(Vector3(bounds.maxs.x, bounds.mins.y), color, Vector2(1.f,0.f));
	vertices[2] = Vertex_3DPCU(Vector3(bounds.mins.x, bounds.maxs.y), color, Vector2(0.f,1.f));
	vertices[3] = Vertex_3DPCU(Vector3(bounds.mins.x, bounds.maxs.y), color, Vector2(0.f,1.f));
	vertices[4] = Vertex_3DPCU(Vector3(bounds.maxs.x, bounds.mins.y), color, Vector2(1.f,0.f));
	vertices[5] = Vertex_3DPCU(Vector3(bounds.maxs.x, bounds.maxs.y), color, Vector2(1.f,1.f));
	
	DrawMeshImmediate(vertices, 6, PRIMITIVE_TRIANGLES);
}

//-----------------------------------------------------------------------------------------------
// Draws a quad that is textured
//
void Renderer::DrawTexturedAABB2(const AABB2& bounds, const Texture& texture, const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs, const Rgba& tint)
{
	SetTexture((Texture*)&texture);

	Vertex_3DPCU vertices[6];
	vertices[0] = Vertex_3DPCU(Vector3(bounds.mins.x, bounds.mins.y), tint, Vector2(texCoordsAtMins.x,texCoordsAtMins.y));
	vertices[1] = Vertex_3DPCU(Vector3(bounds.maxs.x, bounds.mins.y), tint, Vector2(texCoordsAtMaxs.x, texCoordsAtMins.y));
	vertices[2] = Vertex_3DPCU(Vector3(bounds.mins.x, bounds.maxs.y), tint, Vector2(texCoordsAtMins.x, texCoordsAtMaxs.y));
	vertices[3] = Vertex_3DPCU(Vector3(bounds.mins.x, bounds.maxs.y), tint, Vector2(texCoordsAtMins.x, texCoordsAtMaxs.y));
	vertices[4] = Vertex_3DPCU(Vector3(bounds.maxs.x, bounds.mins.y), tint, Vector2(texCoordsAtMaxs.x, texCoordsAtMins.y));
	vertices[5] = Vertex_3DPCU(Vector3(bounds.maxs.x, bounds.maxs.y), tint, Vector2(texCoordsAtMaxs.x, texCoordsAtMaxs.y));
	
	DrawMeshImmediate(vertices, 6, PRIMITIVE_TRIANGLES);
}

//-----------------------------------------------------------------------------------------------
// Draws the text on the screen
//
void Renderer::DrawText2D(const Vector2& drawMins, const std::string& asciiText, float cellHeight, const BitmapFont* font, const Rgba& tint, float aspectScale)
{
	SetTexture((Texture*) &font->m_spriteSheet.GetSpriteSheetTexture());
	float cellWidth = cellHeight * (font->m_baseAspect * aspectScale);
	Vector2 drawMaxs = drawMins + Vector2(cellWidth, cellHeight);
	float cursorMins;
	float cursorMaxs;
	float lineMins = drawMins.y;
	float lineMaxs = drawMaxs.y;
	std::vector<Vertex_3DPCU> verticesToRender;
	
	for (size_t index = 0; index < asciiText.size(); ++index)
	{
		cursorMins = ((float)index * cellWidth) + drawMins.x;
		cursorMaxs = ((float)(index + 1) * cellWidth) + drawMins.x;
		AABB2 uv = font->GetUVsForGlyph(asciiText[index]); // Returns the uv coords for the unicode 
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMins, lineMins), tint, Vector2(uv.mins.x, uv.maxs.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMaxs, lineMins), tint, Vector2(uv.maxs.x, uv.maxs.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMins, lineMaxs), tint, Vector2(uv.mins.x, uv.mins.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMins, lineMaxs), tint, Vector2(uv.mins.x, uv.mins.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMaxs, lineMins), tint, Vector2(uv.maxs.x, uv.maxs.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMaxs, lineMaxs), tint, Vector2(uv.maxs.x, uv.mins.y)));
	}

	DrawMeshImmediate(verticesToRender.data(), (int)verticesToRender.size(), PRIMITIVE_TRIANGLES);
}

//-----------------------------------------------------------------------------------------------
// Draws text in box with given alignment and wrap mode
//
void Renderer::DrawTextInBox2D(const AABB2& textBoxBounds, const std::string& asciiText, float cellHeight, const Vector2& textAligment, WrapMode wrapMode, const BitmapFont* font, const Rgba& tint /*= Rgba::WHITE*/, float aspectScale /*= 1.f */)
{
	// Split the string into lines based on \n
	StringTokenizer tokenizer(asciiText, "\n");
	tokenizer.Tokenize();
	Strings tokens = tokenizer.GetTokens();
	size_t lineIndex = 0;

	float textWidth;
	float paddingX;
	float textHeight = font->GetStringHeight(asciiText, cellHeight, aspectScale);
	float paddingY;
	Vector2 drawMins ;
		
	if(wrapMode == WrapMode::OVERRUN)
	{
		paddingY = (textBoxBounds.maxs.y - textBoxBounds.mins.y - (textHeight / (float)tokens.size())) * textAligment.y;
		for(Strings::reverse_iterator iter = tokens.rbegin(); iter != tokens.rend(); ++iter)
		{
			textWidth = font->GetStringWidth(*iter, cellHeight, aspectScale);
			paddingX = (textBoxBounds.maxs.x - textBoxBounds.mins.x - textWidth) * textAligment.x;
			drawMins = textBoxBounds.mins + Vector2(paddingX, paddingY);
			drawMins.y += (float) lineIndex * cellHeight;
			DrawText2D(drawMins, *iter, cellHeight, font, tint, aspectScale);
			lineIndex++;
		}
	}

	else if(wrapMode == WrapMode::SHRINK_TO_FIT)
	{
		float scaleX;
		float scaleY = cellHeight;
		float boxWidth = (textBoxBounds.maxs.x - textBoxBounds.mins.x) * 0.5f;
		float boxHeight = (textBoxBounds.maxs.y - textBoxBounds.mins.y) * 0.5f;
		textWidth = font->GetStringWidth(asciiText, cellHeight, aspectScale);
		if(textWidth >= boxWidth && textHeight <= boxHeight)
		{
			scaleX = boxWidth / textWidth;
			scaleY = scaleX / aspectScale;
		}
		
		else if(textHeight >= boxHeight && textWidth <= boxWidth)
		{
			scaleY = boxHeight / textHeight;
		}

		else if(textWidth >= boxWidth && textHeight >= boxHeight)
		{
			scaleX = 1.f;
			for(Strings::reverse_iterator iter = tokens.rbegin(); iter != tokens.rend(); ++iter)
			{
				textWidth = font->GetStringWidth(*iter, cellHeight, aspectScale);
				if(textWidth > boxWidth)
				{
					float scale = boxWidth / textWidth;
					if(scale < scaleX)
						scaleX = scale; // Choose the highest scale along width from each line
				}
			}
			scaleY = boxHeight / textHeight;
			if(scaleX < scaleY) // Choose the one that scales down the most
			{
				scaleY = scaleX;
			}
		}

		paddingY = (textBoxBounds.maxs.y - textBoxBounds.mins.y - (scaleY * (float)tokens.size())) * textAligment.y;
		for(Strings::reverse_iterator iter = tokens.rbegin(); iter != tokens.rend(); ++iter)
		{
			textWidth = font->GetStringWidth(*iter, scaleY, aspectScale);
			textHeight = font->GetStringHeight(*iter, scaleY, aspectScale);
			paddingX = (textBoxBounds.maxs.x - textBoxBounds.mins.x - textWidth) * textAligment.x;
			drawMins = textBoxBounds.mins + Vector2(paddingX, paddingY);
			drawMins.y += (float) lineIndex * textHeight;
			DrawText2D(drawMins, *iter, scaleY, font, tint, aspectScale);
			lineIndex++;
		}
	}

	else if(wrapMode == WrapMode::WORD_WRAP)
	{

	}
}

//-----------------------------------------------------------------------------------------------
// Draws text in the world 
//
void Renderer::DrawText3D(const std::string& text, const Matrix44& modelTransform, const BitmapFont* font, const Rgba& tint)
{
	/*Vector2 drawMins(0.f, 1.f);*/ // Always (0,1)
	SetTexture((Texture*) &font->m_spriteSheet.GetSpriteSheetTexture());
	float cursorMins = 0.f;
	float cursorMaxs;
	// Compute offset for text to be centered
	float offsetX = font->GetStringWidth(text, 1.f , 1.f);
	cursorMins = -offsetX * 0.5f;

	// Compute the vertices for each character
	std::vector<Vertex_3DPCU> verticesToRender;
	float cellWidth;
	for (size_t index = 0; index < text.size(); ++index)
	{
		cellWidth = font->GetGlyphAspect(text[index]);

		cursorMins = ((float)index * cellWidth) - offsetX;
		cursorMaxs =  cursorMins + cellWidth;
		AABB2 uv = font->GetUVsForGlyph(text[index]); // Returns the uv coords for the unicode 
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMins, 0), tint, Vector2(uv.mins.x, uv.maxs.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMaxs, 0), tint, Vector2(uv.maxs.x, uv.maxs.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMins, 1), tint, Vector2(uv.mins.x, uv.mins.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMins, 1), tint, Vector2(uv.mins.x, uv.mins.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMaxs, 0), tint, Vector2(uv.maxs.x, uv.maxs.y)));
		verticesToRender.push_back(Vertex_3DPCU(Vector3(cursorMaxs, 1), tint, Vector2(uv.maxs.x, uv.mins.y)));
	}

	DrawMeshImmediate(verticesToRender.data(), (int) verticesToRender.size(), PRIMITIVE_TRIANGLES, modelTransform);
}

//-----------------------------------------------------------------------------------------------
// Draws a cube on the screen
//
void Renderer::DrawCube(const Vector3& center, const Vector3& dimensions, const Rgba& color, const AABB2& uvTop /*= AABB2::ZERO_TO_ONE*/, const AABB2& uvSide /*= AABB2::ZERO_TO_ONE*/, const AABB2& uvBottom /*= AABB2::ZERO_TO_ONE*/)
{
	float dimHalfX = dimensions.x * 0.5f;
	float dimHalfY = dimensions.y * 0.5f;
	float dimHalfZ = dimensions.z * 0.5f;

	Vector3 points[] = {
		Vector3(center.x - dimHalfX, center.y - dimHalfY, center.z - dimHalfZ),
		Vector3(center.x + dimHalfX, center.y - dimHalfY, center.z - dimHalfZ),
		Vector3(center.x + dimHalfX, center.y + dimHalfY, center.z - dimHalfZ), 
		Vector3(center.x - dimHalfX, center.y + dimHalfY, center.z - dimHalfZ),
		Vector3(center.x - dimHalfX, center.y - dimHalfY, center.z + dimHalfZ),
		Vector3(center.x + dimHalfX, center.y - dimHalfY, center.z + dimHalfZ),
		Vector3(center.x + dimHalfX, center.y + dimHalfY, center.z + dimHalfZ), 
		Vector3(center.x - dimHalfX, center.y + dimHalfY, center.z + dimHalfZ)
	};

	Vertex_3DPCU vertices[] = {
		Vertex_3DPCU(points[0], color, Vector2(uvSide.mins.x, uvSide.maxs.y)),
		Vertex_3DPCU(points[1], color, uvSide.maxs),
		Vertex_3DPCU(points[2], color, Vector2(uvSide.maxs.x, uvSide.mins.y)),
		Vertex_3DPCU(points[3], color, uvSide.mins),

		Vertex_3DPCU(points[1], color, Vector2(uvSide.mins.x, uvSide.maxs.y)),
		Vertex_3DPCU(points[5], color, uvSide.maxs),
		Vertex_3DPCU(points[6], color, Vector2(uvSide.maxs.x, uvSide.mins.y)),
		Vertex_3DPCU(points[2], color, uvSide.mins),

		Vertex_3DPCU(points[5], color, Vector2(uvSide.mins.x, uvSide.maxs.y)),
		Vertex_3DPCU(points[4], color, uvSide.maxs),
		Vertex_3DPCU(points[7], color, Vector2(uvSide.maxs.x, uvSide.mins.y)),
		Vertex_3DPCU(points[6], color, uvSide.mins),

		Vertex_3DPCU(points[4], color, Vector2(uvSide.mins.x, uvSide.maxs.y)),
		Vertex_3DPCU(points[0], color, uvSide.maxs),
		Vertex_3DPCU(points[3], color, Vector2(uvSide.maxs.x, uvSide.mins.y)),
		Vertex_3DPCU(points[7], color, uvSide.mins),

		Vertex_3DPCU(points[3], color, Vector2(uvTop.mins.x, uvTop.maxs.y)),
		Vertex_3DPCU(points[2], color, uvTop.maxs),
		Vertex_3DPCU(points[6], color, Vector2(uvTop.maxs.x, uvTop.mins.y)),
		Vertex_3DPCU(points[7], color, uvTop.mins),

		Vertex_3DPCU(points[4], color, Vector2(uvBottom.mins.x, uvBottom.maxs.y)),
		Vertex_3DPCU(points[5], color, uvBottom.maxs),
		Vertex_3DPCU(points[1], color, Vector2(uvBottom.maxs.x, uvBottom.mins.y)),
		Vertex_3DPCU(points[0], color, uvBottom.mins)
	};

	unsigned int indices[] = {
		0,1,3,3,1,2,
		4,5,7,7,5,6,
		8,9,11,11,9,10,
		12,13,15,15,13,14,
		16,17,19,19,17,18,
		20,21,23,23,21,22
	};

	DrawMeshImmediateWithIndices(vertices, 24, indices, 36, PRIMITIVE_TRIANGLES);
}

//-----------------------------------------------------------------------------------------------
// Draws a sprite on the screen
//
void Renderer::DrawSprite(const Sprite& sprite, const Matrix44& transform)
{
	AABB2 uvs = sprite.GetUV();
	Vector2 pivot= sprite.GetPivot();
	AABB2 bounds = sprite.GetBounds();
	
	float left = bounds.mins.x;
	float right = bounds.maxs.x;
	float bottom = bounds.mins.y;
	float top = bounds.maxs.y;

	Vector3 p0(left, bottom); 
	Vector3 p1(right, bottom);
	Vector3 p2(right, top); 
	Vector3 p3(left,top);

	Vertex_3DPCU vertices[] = {
		Vertex_3DPCU(p0, Rgba::WHITE, Vector2(uvs.mins.x, uvs.maxs.y)),
		Vertex_3DPCU(p1, Rgba::WHITE, uvs.maxs),
		Vertex_3DPCU(p2, Rgba::WHITE, Vector2(uvs.maxs.x, uvs.mins.y)),
		Vertex_3DPCU(p3, Rgba::WHITE, uvs.mins)
	};

	unsigned int indices[] = { 0, 1, 3, 3, 1, 2 };

	SetTexture((Texture*) sprite.GetTexture());
	DrawMeshImmediateWithIndices(vertices, 4, indices, 6, PRIMITIVE_TRIANGLES, transform);
}

//-----------------------------------------------------------------------------------------------
// Draws the mesh without wondering about renderer current state
//
void Renderer::DrawMeshImmediate(const Vertex_3DPCU* vertices, int numVerts, DrawPrimitiveType mode, const Matrix44& modelMatrix /*= Matrix44::IDENTITY */) 
{
	// Create the mesh container and set the vertices
	Mesh immediateMesh;
	immediateMesh.SetVertices(numVerts, vertices, Vertex_3DPCU::s_layout);

	// Set the draw instructions as per the draw call
	immediateMesh.SetDrawInstructions(mode, false, 0, numVerts);

	// Draw the mesh
	DrawMesh(&immediateMesh, modelMatrix);
}

//-----------------------------------------------------------------------------------------------
// Draws the mesh without wondering about renderer current state
//
void Renderer::DrawMeshImmediateWithIndices(const Vertex_3DPCU* vertices, int numVerts, const unsigned int* indices, int numIndices, DrawPrimitiveType mode, const Matrix44& modelMatrix /*= Matrix()*/)
{
	// Create a mesh object to store the vertices and indices
	Mesh immediateMesh;
	immediateMesh.SetVertices(numVerts, vertices, Vertex_3DPCU::s_layout);
	immediateMesh.SetIndices(numIndices, indices);

	// Set the draw instructions
	immediateMesh.SetDrawInstructions(mode, true, 0, numIndices);

	// Draw the mesh
	DrawMesh(&immediateMesh, modelMatrix);
}

//-----------------------------------------------------------------------------------------------
// Draws a mesh with the model matrix
//
void Renderer::DrawMesh(Mesh* mesh, const Matrix44& modelMatrix /*= Matrix44::IDENTITY */)
{
	GLenum drawMode = GetGLPrimitive(mesh->m_drawInstruction.m_drawType); // Get the actual GL primitive
	
	// Bind uniforms
	// Binds projection matrix on the shader 
	m_activeMaterial->SetProperty("PROJECTION", m_currentCamera->m_projMatrix);

	// Binds view matrix on the shader
	m_activeMaterial->SetProperty("VIEW", m_currentCamera->m_viewMatrix);

	// Binds model matrix on the shader
	m_activeMaterial->SetProperty("MODEL", modelMatrix);

	// Binds the eye position uniform
	m_activeMaterial->SetProperty("EYE_POSITION", m_currentCamera->m_transform.GetWorldPosition());

	// Binds the render mode uniform
	m_activeMaterial->SetProperty("RENDER_MODE", m_renderMode);

	// Bind the game time uniform
	m_activeMaterial->SetProperty("GAME_TIME", (float) Clock::GetMasterClock()->GetTime());

	// Binds the active material
	BindMaterial(m_activeMaterial);

	// Bind the mesh to the currently bound shader program
	GLuint programHandle = m_activeMaterial->GetShader()->GetProgram()->GetHandle();
	BindMeshToProgram(programHandle, mesh);

	// Binds the light buffer
	m_lightBuffer->Set<LightBlock>(*m_lightBlock);
	m_lightBuffer->UpdateGPU();
	BindUBO(BLOCK_LIGHT, m_lightBuffer);

	// Bind the specular block
	m_specularBuffer->Set<SpecularBlock>(*m_specularBlock);
	m_specularBuffer->UpdateGPU();
	BindUBO(BLOCK_SPECULAR, m_specularBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, m_currentCamera->GetFrameBufferHandle());

	if(mesh->m_drawInstruction.m_useIndices)
	{
		glDrawElements(drawMode, mesh->m_drawInstruction.m_elementCount, GL_UNSIGNED_INT, (GLvoid*) mesh->m_drawInstruction.m_startIndex);
	}
	else
	{
		glDrawArrays(drawMode, (int) mesh->m_drawInstruction.m_startIndex, mesh->m_drawInstruction.m_elementCount);
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a mesh or gets the existing instance of a mesh
//
Mesh* Renderer::CreateOrGetMesh(const std::string& path)
{
	if(m_loadedMeshes.find(path) != m_loadedMeshes.end())
	{
		return m_loadedMeshes.at(path);
	}
	else
	{
		Mesh* mesh = new Mesh();
		mesh->FromFile<VertexLit>(path.c_str());
		m_loadedMeshes[path] = mesh;
		return mesh;
	}
}

//-----------------------------------------------------------------------------------------------
// Creates default meshes like sphere, cube, plane and stores it in the loaded meshes
//
void Renderer::InitializeDefaultMeshes()
{
	Mesh* cube = CreateCube(Vector3::ZERO, Vector3::ONE);
	m_loadedMeshes["Cube"] = cube;

	Mesh* sphere = CreateUVSphere(Vector3::ZERO, 1.f, 25, 25);
	m_loadedMeshes["Sphere"] = sphere;

	Mesh* plane = CreateQuad3D(Vector2::ZERO, Vector3::RIGHT, Vector3::UP, Vector2::ONE);
	m_loadedMeshes["Quad"] = plane;
}

//-----------------------------------------------------------------------------------------------
// Sets the texture on the default material
//
void Renderer::SetTexture(Texture* texture)
{
	if(!texture)
	{
		SetTexture(0, m_defaultTexture);
	}
	
	SetTexture(0, texture);
}

//-----------------------------------------------------------------------------------------------
// Sets the texture on the default material
//
void Renderer::SetTexture(unsigned int index, Texture* texture, Sampler* sampler)
{
	m_defaultMaterial->SetTexture(index, texture, sampler);
}

//-----------------------------------------------------------------------------------------------
// Returns the texture id. 
//
Texture* Renderer::CreateOrGetTexture(const std::string& path, bool genMipmaps)
{
	if(m_loadedTextures.find(path) != m_loadedTextures.end())
	{
		return m_loadedTextures.at(path);
	}
	else
	{
		Texture* newTexture = new Texture(path, nullptr, genMipmaps);
		m_loadedTextures[path] = newTexture;
		return newTexture;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns texture id 
//
Texture* Renderer::CreateOrGetTexture(const Image& image, bool genMipmaps)
{
	if(m_loadedTextures.find(image.GetPath()) != m_loadedTextures.end())
	{
		return m_loadedTextures.at(image.GetPath());
	}
	else
	{
		Texture* newTexture = new Texture(image, nullptr, genMipmaps);
		m_loadedTextures[image.GetPath()] = newTexture;
		return newTexture;
	}
}

//-----------------------------------------------------------------------------------------------
// Checks if texture is already loaded
//
bool Renderer::IsTextureLoaded(const std::string& path) const
{
	if(m_loadedTextures.find(path) != m_loadedTextures.end())
	{
		return true;
	}
	return false;
	
}

//-----------------------------------------------------------------------------------------------
// Binds the texture 2D to slot 0
//
void Renderer::BindTexture2D(const Texture* texture)
{
	if(texture == nullptr)
	{
		BindTexture2D(0, m_defaultTexture);
		return;
	}

	BindTexture2D(0U, texture);
}

//-----------------------------------------------------------------------------------------------
// Binds the texture 2D to the slot specified
//
void Renderer::BindTexture2D(unsigned int index, const Texture* texture)
{
	if(!texture)
	{
		texture = m_defaultTexture;
	}

	// Bind the sampler;
	glBindSampler( index, texture->GetSamplerHandle() ); 

	// Bind the texture
	glActiveTexture( GL_TEXTURE0 + index ); 
	glBindTexture( GL_TEXTURE_2D, texture->m_textureID ); 
}

//-----------------------------------------------------------------------------------------------
// Binds the default texture
//
void Renderer::SetDefaultTexture()
{
	SetTexture(m_defaultTexture);
}

//-----------------------------------------------------------------------------------------------
// Copies from src texture in the destination using frame buffers
//
void Renderer::CopyTexture2D(Texture* dest, Texture* src)
{
	m_copyDestination->SetColorTarget(dest); // Assigns to framebuffer
	m_copySrc->SetColorTarget(src);

	m_copyDestination->SetDepthStencilTarget(m_defaultDepthTarget);
	m_copySrc->SetDepthStencilTarget(m_defaultDepthTarget);

	CopyFrameBuffer(m_copyDestination, m_copySrc); // Blits framebuffer 
}

//-----------------------------------------------------------------------------------------------
// Alpha blending mode setter
//
void Renderer::BlendFunction(BlendFactor sfactor , BlendFactor dfactor )
{
	m_defaultShader->SetAlphaBlending(BLEND_OP_ADD, sfactor, dfactor);
	m_defaultMaterial->SetAlphaBlending(BLEND_OP_ADD, sfactor, dfactor);
}

//-----------------------------------------------------------------------------------------------
// Color blend function setter
//
void Renderer::ColorBlendFunction(BlendFactor sfactor, BlendFactor dfactor)
{
	m_defaultShader->SetColorBlending(BLEND_OP_ADD, sfactor, dfactor);
	m_defaultMaterial->SetColorBlending(BLEND_OP_ADD, sfactor, dfactor);
}

//-----------------------------------------------------------------------------------------------
// Wrapper for line width function
//
void Renderer::LineWidth(float width) const
{
	glLineWidth(width);
}

//-----------------------------------------------------------------------------------------------
// Enables depth testing 
//
void Renderer::SetDepthTestMode(DepthTestOp mode, bool flag)
{
	m_defaultShader->SetDepthTest(mode, flag);
	m_defaultMaterial->SetDepthTest(mode, flag);
}

//-----------------------------------------------------------------------------------------------
// Disables depth
//
void Renderer::DisableDepth()
{
	SetDepthTestMode(COMPARE_ALWAYS, false);
}

//-----------------------------------------------------------------------------------------------
// Returns the GL primitive mode from the user-define DrawPrimitive
//
unsigned int Renderer::GetGLPrimitive(DrawPrimitiveType mode) const
{
	switch(mode)
	{
	case PRIMITIVE_LINES: 				return GL_LINES;
	case PRIMITIVE_LINE_LOOP:			return GL_LINE_LOOP;
	case PRIMITIVE_TRIANGLES:			return GL_TRIANGLES;
	case PRIMITIVE_TRIANGLE_STRIP:		return GL_TRIANGLE_STRIP;
	case PRIMITIVE_POINT:				return GL_POINT;
	default:
		GUARANTEE_OR_DIE(false, "Invalid Draw primitive ");
	}
}


//-----------------------------------------------------------------------------------------------
// Returns the GLEnum for the depth test mode
//
unsigned int Renderer::GetGLDepthTestMode(DepthTestOp mode) const
{
	switch(mode)
	{
	case COMPARE_ALWAYS:		return GL_ALWAYS;
	case COMPARE_GEQUAL:		return GL_GEQUAL;
	case COMPARE_GREATER:		return GL_GREATER;
	case COMPARE_LEQUAL:		return GL_LEQUAL;
	case COMPARE_LESS:			return GL_LESS;
	case COMPARE_NEVER:			return GL_NEVER;
	case COMPARE_NOTEQUAL:		return GL_NOTEQUAL;
	default:
		GUARANTEE_OR_DIE(false, "Invalid Depth Test Mode");
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GL data type 
//
unsigned int Renderer::GetGLDataType(RenderType type) const
{
	switch (type)
	{
	case RT_FLOAT:				return GL_FLOAT;
	case RT_INT:				return GL_INT;
	case RT_UNSIGNED_BYTE:		return GL_UNSIGNED_BYTE;
	case RT_UNSIGNED_INT:		return GL_UNSIGNED_INT;
	default:
		GUARANTEE_OR_DIE(false, "Invalid Data Type");
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GL blend operation enum
//
unsigned int Renderer::GetGLBlendOp(BlendOp mode) const
{
	switch (mode)
	{
	case BLEND_OP_ADD:				return GL_FUNC_ADD;					
	case BLEND_OP_SUBTRACT:			return GL_FUNC_SUBTRACT;			
	case BLEND_OP_REVERSE_SUBTRACT:	return GL_FUNC_REVERSE_SUBTRACT;	
	case BLEND_OP_MIN:				return GL_MIN;						
	case BLEND_OP_MAX:				return GL_MAX;						
	default:
		GUARANTEE_OR_DIE(false, "Invalid Blend op");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GL blend factor enum
//
unsigned int Renderer::GetGLBlendFactor(BlendFactor factor) const
{
	switch (factor)
	{
	case BLEND_ZERO:					return GL_ZERO;						
	case BLEND_ONE:						return GL_ONE;						
	case BLEND_ONE_MINUS_SRC_ALPHA:		return GL_ONE_MINUS_SRC_ALPHA; 		
	case BLEND_SRC_ALPHA:				return GL_SRC_ALPHA;
	case BLEND_SRC_COLOR:				return GL_SRC_COLOR;
	case BLEND_ONE_MINUS_SRC_COLOR:		return GL_ONE_MINUS_SRC_COLOR;
	case BLEND_DST_ALPHA:				return GL_DST_ALPHA;
	default:
		GUARANTEE_OR_DIE(false, "Invalid Blend factor");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GL wind order enum
//
unsigned int Renderer::GetGLWindOrder(WindOrder order) const
{
	switch (order)
	{
	case WIND_CLOCKWISE:			return GL_CW;
	case WIND_COUNTER_CLOCKWISE:	return GL_CCW;
	default:
		GUARANTEE_OR_DIE(false, "Invalid Wind order");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GL fill mode enum
//
unsigned int Renderer::GetGLFillMode(FillMode mode) const
{
	switch (mode)
	{
	case FILLMODE_SOLID: return GL_FILL;
	case FILLMODE_WIRE:	 return GL_LINE;
	default:
		GUARANTEE_OR_DIE(false, "Invalid fill mode");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GL cull mode enum
//
unsigned int Renderer::GetGLCullMode(CullMode mode) const
{
	switch (mode)
	{
	case CULLMODE_BACK:		return GL_BACK;
	case CULLMODE_FRONT:	return GL_FRONT;
	case CULLMODE_NONE:		return GL_NONE;
	default:
		GUARANTEE_OR_DIE(false, "Invalid cull mode");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a render target 
//
Texture* Renderer::CreateRenderTarget(unsigned int width, unsigned int height, eTextureFormat fmt /*= TEXTURE_FORMAT_RGBA8*/)
{
	Texture* target = new Texture();
	target->CreateRenderTarget(width, height, fmt);
	return target;
}

//-----------------------------------------------------------------------------------------------
// Creates a Depth stencil target 
//
Texture* Renderer::CreateDepthStencilTarget(unsigned int width, unsigned int height)
{
	return CreateRenderTarget(width, height, TEXTURE_FORMAT_D24S8);
}

//-----------------------------------------------------------------------------------------------
// Creates or returns reference to a cubemap texture
//
TextureCube* Renderer::CreateOrGetCubeMap(const std::string& path)
{
	std::map<std::string, TextureCube*>::iterator found = m_loadedCubemaps.find(path);
	if(found != m_loadedCubemaps.end())
	{
		return found->second;
	}
	else 
	{
		TextureCube* cubemap = new TextureCube(path);
		m_loadedCubemaps[path] = cubemap;
		return cubemap;
	}
}

//-----------------------------------------------------------------------------------------------
// Binds the cubemap 
//
void Renderer::BindCubemap(const TextureCube* cubemap)
{
	// Bind the sampler;
	glBindSampler( TEXTURE_SLOT_SKYBOX, Sampler::GetLinearSampler()->GetHandle() ); 

	// Bind the texture
	glActiveTexture( GL_TEXTURE0 + TEXTURE_SLOT_SKYBOX ); 
	glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap->m_handle ); 
}

//-----------------------------------------------------------------------------------------------
// Creates or returns an instance of the gif path specified
//
GIFAnimation* Renderer::AcquireGIFInstance(const std::string& path)
{
	GIFAnimation* instance;
	if(m_loadedGIFs.find(path) != m_loadedGIFs.end())
	{
		instance = new GIFAnimation(*m_loadedGIFs[path]);
	}
	else
	{
		GIFAnimation* newGIF = new GIFAnimation(path);
		m_loadedGIFs[path] = newGIF;
		instance = new GIFAnimation(*m_loadedGIFs[path]);
	}

	return instance;
}

//-----------------------------------------------------------------------------------------------
// Writes into another FBO (if dst is nullptr, writes to back-buffer)
//
bool Renderer::CopyFrameBuffer(FrameBuffer* dest, FrameBuffer* src)
{
	// we need at least the src.
	if (src == nullptr) {
		return false; 
	}

	// Get the handles - NULL refers to the "default" or back buffer FBO
	GLuint srcFbo = src->GetHandle();
	GLuint destFbo = NULL; 
	if (dest != nullptr) {
		destFbo = dest->GetHandle(); 
	}

	// can't copy onto ourselves
	if (destFbo == srcFbo) {
		return false; 
	}

	// the GL_READ_FRAMEBUFFER is where we copy from
	glBindFramebuffer( GL_READ_FRAMEBUFFER, srcFbo ); 
	GL_CHECK_ERROR();

	// what are we copying to?
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, destFbo ); 
	GL_CHECK_ERROR();

	// blit it over - get teh size
	// (we'll assume dst matches for now - but to be safe,
	// you should get dst_width and dst_height using either
	// dst or the window depending if dst was nullptr or not
	unsigned int width = src->m_width;     
	unsigned int height = src->m_height; 

	// Copy it over
	glBlitFramebuffer( 0, 0, // src start pixel
		width, height,        // src size
		0, 0,                 // dst start pixel
		width, height,        // dst size
		GL_COLOR_BUFFER_BIT,  // what are we copying (just colour)
		GL_NEAREST );         // resize filtering rule (in case src/dst don't match)

	GL_CHECK_ERROR();
							  // cleanup after ourselves
	glBindFramebuffer( GL_READ_FRAMEBUFFER, NULL ); 
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, NULL ); 

	return GLSucceeded();
}

//-----------------------------------------------------------------------------------------------
// Sets the camera if specified, else default camera
//
void Renderer::SetCamera(Camera* cam)
{
	if(cam == nullptr)
	{
		cam = m_defaultCamera;
	}
	IntVector2 viewMins = IntVector2(cam->GetViewportMins());
	IntVector2 viewMaxs = IntVector2(cam->GetViewportMaxs());
	glViewport(viewMins.x, viewMins.y, viewMaxs.x, viewMaxs.y);

	cam->Finalize();
	m_currentCamera = cam;
}

//-----------------------------------------------------------------------------------------------
// Takes the screenshot of the current frame
//
void Renderer::TakeScreenshot(const char* fileName)
{
	
	int width = m_currentCamera->m_frameBuffer->GetWidth();
	int height = m_currentCamera->m_frameBuffer->GetHeight();
	glReadBuffer(GL_FRONT);
	std::vector<GLubyte> buffer(width * height * 4);
 	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
	
	std::string folderName = SOLUTION_DIR;
	folderName.append(R"(Screenshots\)");

	if(!CreateDirectoryA(folderName.c_str() ,NULL)) //Create the directory if not available
	{
	}
	
	if(WriteToPng(fileName, buffer.data(), width, height, 4))
	{
		ConsolePrintf("Screenshot saved to %s",fileName);
	}
	buffer.clear();
}

//-----------------------------------------------------------------------------------------------
// Returns the bitmap font id
// 
BitmapFont* Renderer::CreateOrGetBitmapFont( const char* bitmapFontName )
{
	std::string path = Stringf("Data/Fonts/%s.png", bitmapFontName);
	std::map<std::string, BitmapFont*>::iterator found = m_loadedFonts.find(bitmapFontName);
	if(found != m_loadedFonts.end())
	{
		return found->second;
	}
	else 
	{
		SpriteSheet* bitMapsheet = new SpriteSheet(*CreateOrGetTexture(path, false), BITMAP_FONT_TILES_WIDE, BITMAP_FONT_TILES_HIGH);
		BitmapFont* newFont = new BitmapFont( bitmapFontName, *bitMapsheet, BITMAP_FONT_DEFAULT_BASE_ASPECT );
		m_loadedFonts[bitmapFontName] = newFont;
		return newFont;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns true if the font is already in memory
//
bool Renderer::IsBitmapFontLoaded( const std::string& bitmapFontName ) const
{
	if(m_loadedFonts.find(bitmapFontName) != m_loadedFonts.end())
	{
		return true;
	}
	return false;

}

//-----------------------------------------------------------------------------------------------
// Sets the current shader
//
void Renderer::SetShader(Shader* shader)
{
	if(shader == nullptr)
	{
		BindDefaultShader();
	}
	
	else
	{
		m_defaultMaterial->SetShader(shader);
	}
}

//-----------------------------------------------------------------------------------------------
// Creates the shader program or returns the handle if its already loaded
//
ShaderProgram* Renderer::CreateOrGetShaderProgram(const std::string& path, const char* defines /*= nullptr*/)
{
	if(path.compare("default") == 0)
	{
		return g_defaultProgram; // Return default shader when requested
	}

	else if(path.compare("diffuse") == 0)
	{
		return g_defDiffuseProgram;
	}

	else
	{
		std::map<std::string, ShaderProgram*>::const_iterator found = m_loadedShaderPrograms.find(path);
		if (found != m_loadedShaderPrograms.end())
		{
			if(found->second == nullptr)
				return g_invalidProgram;

			return found->second;
		}
		else
		{
			ShaderProgram* newShader = new ShaderProgram();
			bool status = newShader->LoadFromFiles(path.c_str(), defines);
			if (status)
			{
				m_loadedShaderPrograms[path] = newShader;
				return newShader;
			}
			else
			{
				m_loadedShaderPrograms[path] = nullptr; // Puts it in the map so that it can be reloaded with the key
				delete newShader;
				return g_invalidProgram;		 // Returns invalid shader when there is an error 
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the current shader to be used to draw
//
void Renderer::UseShaderProgram(const ShaderProgram* shaderProgram)
{
	const ShaderProgram* current = shaderProgram;
	if(!current)
	{
		current = g_defaultProgram;
	}
	
	glUseProgram(current->GetHandle());
}

//-----------------------------------------------------------------------------------------------
// Sets the active shader as the default shader
//
void Renderer::BindDefaultShader()
{
	m_defaultMaterial->SetShader(m_defaultShader);
}

//-----------------------------------------------------------------------------------------------
// Binds the vertex layout to a shader program
//
void Renderer::BindMeshToProgram(unsigned int programHandle, const Mesh* mesh)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo->GetHandle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ibo->GetHandle());

	const VertexLayout* layout = mesh->GetLayout();
	for(int attribIndex = 0; attribIndex < layout->m_attributes.size(); attribIndex++)
	{
		const VertexAttribute* attrib = layout->GetAttribute(attribIndex);
		GLenum dataType = GetGLDataType(attrib->m_type);
		GLint bind = glGetAttribLocation(programHandle, attrib->m_handle);
		if(bind >= 0)
		{
			glEnableVertexAttribArray(bind);
			glVertexAttribPointer(bind,				// Where?
				attrib->m_elementCount,				// How many ?
				dataType,							// What's the data type?
				attrib->m_isNormalized,				// Should the data be normalized?
				layout->m_stride,					// How much space between each vertex
				(GLvoid*) attrib->m_memberOffset);	// How far away is the needed data from the start of each?
			GL_CHECK_ERROR();
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Binds the render state from the active shader
//
void Renderer::BindRenderState(RenderState state)
{
	GL_CHECK_ERROR();
	// Blend mode
	glBlendEquationSeparate(GetGLBlendOp(state.m_colorBlendOp), GetGLBlendOp(state.m_alphaBlendOp));
	glBlendFuncSeparate(GetGLBlendFactor(state.m_colorSrcFactor), GetGLBlendFactor(state.m_colorDstFactor), 
						GetGLBlendFactor(state.m_alphaSrcFactor), GetGLBlendFactor(state.m_alphaDstFactor));
	GL_CHECK_ERROR();

	// Depth mode
	glDepthFunc(GetGLDepthTestMode(state.m_depthCompare));
	glDepthMask(state.m_depthWrite);
	GL_CHECK_ERROR();

	// Wind order
	glFrontFace(GetGLWindOrder(state.m_frontFace));
	GL_CHECK_ERROR();

	// Culling
	if(state.m_cullMode == CULLMODE_NONE)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GetGLCullMode(state.m_cullMode));
	}
	
	GL_CHECK_ERROR();

	// Fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(state.m_fillMode));
	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
// Compiles and Reloads all shaders currently loaded except default and invalid
//
void Renderer::ReloadShaderPrograms()
{
	std::map<std::string, ShaderProgram*>::iterator found = m_loadedShaderPrograms.begin();

	while(found != m_loadedShaderPrograms.end())
	{
		if(found->second)
		{
			glDeleteProgram(found->second->GetHandle()); // Remove it from GPU 
			found->second->m_programHandle = NULL;
		}

		else
		{
			found->second = new ShaderProgram();
		}

		bool status = found->second->LoadFromFiles(found->first.c_str()); // Reload it from the file 
		if(!status)
		{
			// Invalid again
			delete found->second;
			found->second = nullptr;
		}

		++found;
	}
}

//-----------------------------------------------------------------------------------------------
// Binds the material and its properties, textures, etc
//
void Renderer::BindMaterial(const Material* material /*= nullptr */)
{
	// Bind the shader
	UseShaderProgram(material->GetShader()->GetProgram());
	BindRenderState(material->GetShader()->m_renderState);

	// Bind the textures and samplers on the material
	for(size_t texIdx = 0; texIdx < material->m_textures.size(); ++texIdx)
	{
		BindTexture2D((int) texIdx, material->m_textures[texIdx]);
	}

	// Bind the material properties
	std::map<std::string, MaterialProperty*>::const_iterator iter = material->m_properties.begin();
	for(; iter != material->m_properties.end(); ++iter )
	{
		iter->second->Bind( material->GetShader()->GetProgram()->GetHandle() );
	}

	// Specular block
	m_specularBlock->specFactor = material->m_specFactor;
	m_specularBlock->specPower = material->m_specPower;
}

//-----------------------------------------------------------------------------------------------
// Sets the active material
//
void Renderer::SetMaterial(const Material* material /*= nullptr */)
{
	m_activeMaterial = const_cast<Material*>(material);
}

//-----------------------------------------------------------------------------------------------
// Creates or returns the material 
//
Material* Renderer::CreateOrGetMaterial(const std::string& path)
{
	if(m_loadedMaterials.find(path) != m_loadedMaterials.end())
	{
		return m_loadedMaterials.at(path);
	}
	else
	{
		Material* newMaterial = new Material(path);
		m_loadedMaterials[path] = newMaterial;
		return newMaterial;
	}
}

//-----------------------------------------------------------------------------------------------
// Binds the default material 
//
void Renderer::SetDefaultMaterial()
{
	m_activeMaterial = m_defaultMaterial;
}

//-----------------------------------------------------------------------------------------------
// Resets the default material 
//
void Renderer::ResetDefaultMaterial()
{
	delete m_defaultMaterial;
	m_defaultMaterial = m_defaultMaterialShared->Clone();
}

//-----------------------------------------------------------------------------------------------
// Sets the ambient light with a color and intensity
//
void Renderer::SetAmbientLight(const Rgba& color, float intensity /*= 1.f */)
{
	m_lightBlock->ambient = color.GetAsVector();
	m_lightBlock->ambient.a = intensity;
}

//-----------------------------------------------------------------------------------------------
// Disables the ambient light in the scene
//
void Renderer::DisableAmbientLight()
{
	m_lightBlock->ambient.a = 0.f;
}

//-----------------------------------------------------------------------------------------------
// Increases intensity of the ambient light
//
void Renderer::IncreaseAmbientLightIntensity(float amount)
{
	m_lightBlock->ambient.a += amount;
	m_lightBlock->ambient.a = ClampFloatZeroToOne(m_lightBlock->ambient.a);
}

//-----------------------------------------------------------------------------------------------
// Decreases ambient light intensity
//
void Renderer::DecreaseAmbientLightIntensity(float amount)
{
	m_lightBlock->ambient.a -= amount;
	m_lightBlock->ambient.a = ClampFloatZeroToOne(m_lightBlock->ambient.a);
}

//-----------------------------------------------------------------------------------------------
// Enables a point light in the scene
//
void Renderer::EnablePointLight(int index, const Vector3& position, const Rgba& color /*= Rgba::WHITE*/, float intensity /*= 1.f*/, const Vector3& attenuation /*= Vector3(0.f, 1.f, 0.f) */)
{
	LightStructure& light = m_lightBlock->lights[index];
	light.position = position;
	light.color = color.GetAsVector();
	light.color.w = intensity;
	light.direction = Vector3::DOWN;
	light.directionFactor = 0.f;
	light.spotFactor = 0.f;
	light.attenuation = attenuation;
}

//-----------------------------------------------------------------------------------------------
// Enables a directional light in the scene
//
void Renderer::EnableDirectionalLight(int index, const Vector3& position, const Vector3& dir, const Rgba& color /*= Rgba::WHITE*/, float intensity /*= 1.f*/)
{
	LightStructure& light = m_lightBlock->lights[index];
	light.position = position;
	light.color = color.GetAsVector();
	light.color.w = intensity;
	light.direction = dir;
	light.directionFactor = 1.f;
	light.spotFactor = 0.f;
	light.attenuation = Vector3(1.f, 0.f, 0.f);
}

//-----------------------------------------------------------------------------------------------
// Enables a spot light in the scene 
//
void Renderer::EnableSpotlight(int index, const Vector3& position, const Vector3& dir, float innerAngle, float outerAngle, const Rgba& color /*= Rgba::WHITE*/, float intensity /*= 1.f */)
{
	LightStructure& light = m_lightBlock->lights[index];
	light.position = position;
	light.color = color.GetAsVector();
	light.color.w = intensity;
	light.direction = dir;
	light.directionFactor = 1.f;
	light.spotFactor = 1.f;
	light.dotInnerAngle = CosDegrees(innerAngle);
	light.dotOuterAngle = CosDegrees(outerAngle);
	light.attenuation = Vector3(1.f, 0.f, 0.0f);
}

//-----------------------------------------------------------------------------------------------
// Sets the light position
//
void Renderer::SetLightPosition(int index, const Vector3& position)
{
	LightStructure& light = m_lightBlock->lights[index];
	light.position = position;
}

//-----------------------------------------------------------------------------------------------
// Sets the direction on the light at index
//
void Renderer::SetLightDirection(int index, const Vector3& dir)
{
	LightStructure& light = m_lightBlock->lights[index];
	light.direction = dir;
}

//-----------------------------------------------------------------------------------------------
// Disables all point lights in the scene
//
void Renderer::DisableAllLights()
{
	int index = 0;
	for(; index < MAX_LIGHTS; index++)
	{
		m_lightBlock->lights[index].color = Vector4(0.f,0.f,0.f,0.f);
		m_lightBlock->lights[index].directionFactor = 0.f;
		m_lightBlock->lights[index].spotFactor = 0.f;
	}
}

//-----------------------------------------------------------------------------------------------
// Disables the light at index
//
void Renderer::DisableLight(int index)
{
	m_lightBlock->lights[index].color.w = 0.f;
}

//-----------------------------------------------------------------------------------------------
// Sets the specular factor on the specular block
//
void Renderer::SetSpecularFactor(float factor)
{
	m_defaultMaterial->SetSpecFactor(factor);
}

//-----------------------------------------------------------------------------------------------
// Sets the specular power on the specular block
//
void Renderer::SetSpecularPower(float power)
{
	m_defaultMaterial->SetSpecPower(power);
}

//-----------------------------------------------------------------------------------------------
// Sets the light buffer from the vector of lights
//
void Renderer::SetLightBuffer(std::vector<Light*>& lights)
{
	DisableAllLights();

	for(size_t index = 0; index < lights.size(); ++index)
	{
		Light* light = lights[index];
		LightStructure* lightBuf = &m_lightBlock->lights[index];
		
		lightBuf->attenuation = light->GetAttenuation();
		lightBuf->position = light->GetWorldPosition();
		lightBuf->color = light->GetColor().GetAsVector();
		lightBuf->color.a = light->GetIntensity();
		lightBuf->direction = light->GetDirection();
		lightBuf->directionFactor = light->GetDirFactor();
		lightBuf->spotFactor = light->GetSpotFactor();
		lightBuf->dotInnerAngle = light->GetInnerDot();
		lightBuf->dotOuterAngle = light->GetOuterDot();
		lightBuf->isShadowCasting = light->m_lightDesc.isShadowCasting;
		lightBuf->shadowVP = light->m_lightDesc.shadowVP;
	}
}

//-----------------------------------------------------------------------------------------------
// Binds a Uniform Buffer to the current shader program
//
void Renderer::BindUBO(int bindPoint, const UniformBuffer* ubo)
{
	GL_CHECK_ERROR();
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, ubo->GetHandle());
	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
// Sets a float uniform on the currently bound shader
//
void Renderer::SetUniform(const char* name, float value)
{
	m_defaultMaterial->SetProperty(name, value);
}

//-----------------------------------------------------------------------------------------------
// Sets an integer uniform on the currently bound shader
//
void Renderer::SetUniform(const char* name, int value)
{
	m_defaultMaterial->SetProperty(name, value);
}

//-----------------------------------------------------------------------------------------------
// Sets a matrix uniform on the currently bound shader
//
void Renderer::SetUniform(const char* name, const Matrix44& matrix, bool transpose)
{
	GL_CHECK_ERROR();
	m_defaultMaterial->SetProperty(name, matrix, transpose);
}

//-----------------------------------------------------------------------------------------------
// Sets a color on the currently bound shader
//
void Renderer::SetUniform(const char* name, const Rgba& color)
{
	GL_CHECK_ERROR();
	m_defaultMaterial->SetProperty(name, color);
}

//-----------------------------------------------------------------------------------------------
// Sets a Vec3 on the currently bound shader
//
void Renderer::SetUniform(const char* name, const Vector3& value)
{
	m_defaultMaterial->SetProperty(name, value);
}

//-----------------------------------------------------------------------------------------------
// Applies a framebuffer effect using the shaderprogram specified
//
void Renderer::ApplyEffect(const ShaderProgram* shaderProgram)
{
	if(!m_effectTarget)
	{
		m_effectTarget = m_defaultColorTarget;
		if(!m_effectScratch)
		{
			m_effectScratch = Texture::CreateCompatibleTarget(m_effectTarget);
		}
	}
	m_effectCamera->SetColorTarget(m_effectScratch); // scratch to make changes to
	SetCamera(m_effectCamera);

	UseShaderProgram(shaderProgram);
	BindTexture2D(m_effectTarget); // Current frame 
	AABB2 bounds(-1.f * Vector2::ONE, Vector2::ONE);
	DrawAABB2(bounds);

	// Effect added to scratch. Now switch it with the main target
	std::swap(m_effectTarget, m_effectScratch); 
}

//-----------------------------------------------------------------------------------------------
// Finishes the frame buffer effects that are being added
//
void Renderer::FinishEffects()
{
	if(!m_effectTarget)
		return; // No effects done

	if(m_effectTarget != m_defaultColorTarget)
	{
		// Odd number of effects. We need to copy the affected color target to default 
		CopyTexture2D( m_defaultColorTarget, m_effectTarget ); 
		m_effectScratch = m_effectTarget;
	}
	m_effectTarget = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Sets the fill mode 
//
void Renderer::SetFillMode(FillMode fillMode)
{
	m_defaultShader->SetFillMode(fillMode);
	m_defaultMaterial->SetFillMode(fillMode);
}

//-----------------------------------------------------------------------------------------------
// Sets the cull mode on the default shader
//
void Renderer::SetCullMode(CullMode cullMode)
{
	m_defaultShader->SetCullMode(cullMode);
	m_defaultMaterial->SetCullMode(cullMode);
}

//------------------------------------------------------------------------
// Creates the old rendering context to get bind the new WGL functions
//
static HGLRC CreateOldRenderContext( HDC hdc ) 
{
	// Setup the output to be able to render how we want
	// (in our case, an RGBA (4 bytes per channel) output that supports OpenGL
	// and is double buffered
	PIXELFORMATDESCRIPTOR pfd;
	memset( &pfd, 0, sizeof(pfd) ); 
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 0; // 24; Depth/Stencil handled by FBO
	pfd.cStencilBits = 0; // 8; DepthStencil handled by FBO
	pfd.iLayerType = PFD_MAIN_PLANE; // ignored now according to MSDN

									 // Find a pixel format that matches our search criteria above. 
	int pixel_format = ::ChoosePixelFormat( hdc, &pfd );
	if ( pixel_format == NULL ) {
		return NULL; 
	}

	// Set our HDC to have this output. 
	if (!::SetPixelFormat( hdc, pixel_format, &pfd )) {
		return NULL; 
	}

	
	// Create the context for the HDC
	HGLRC context = wglCreateContext( hdc );
	if (context == NULL) {
		return NULL; 
	}

	// return the context; 
	return context; 
}

//------------------------------------------------------------------------
// Creates a real context as a specific version (major.minor)
//
static HGLRC CreateRealRenderContext( HDC hdc, int major, int minor ) 
{
	// So similar to creating the temp one - we want to define 
	// the style of surface we want to draw to.  But now, to support
	// extensions, it takes key_value pairs
	int const format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,    // The rc will be used to draw to a window
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,    // ...can be drawn to by GL
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,     // ...is double buffered
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // ...uses a RGBA texture
		WGL_COLOR_BITS_ARB, 24,             // 24 bits for color (8 bits per channel)
											// WGL_DEPTH_BITS_ARB, 24,          // if you wanted depth a default depth buffer...
											// WGL_STENCIL_BITS_ARB, 8,         // ...you could set these to get a 24/8 Depth/Stencil.
											NULL, NULL,                         // Tell it we're done.
	};

	// Given the above criteria, we're going to search for formats
	// our device supports that give us it.  I'm allowing 128 max returns (which is overkill)
	size_t const MAX_PIXEL_FORMATS = 128;
	int formats[MAX_PIXEL_FORMATS];
	int pixel_format = 0;
	UINT format_count = 0;

	BOOL succeeded = wglChoosePixelFormatARB( hdc, 
		format_attribs, 
		nullptr, 
		MAX_PIXEL_FORMATS, 
		formats, 
		(UINT*)&format_count );

	if (!succeeded) {
		return NULL; 
	}

	// Loop through returned formats, till we find one that works
	for (GLuint i = 0; i < format_count; ++i) {
		pixel_format = formats[i];
		succeeded = SetPixelFormat( hdc, pixel_format, NULL ); // same as the temp context; 
		if (succeeded) {
			break;
		} else {
			//DWORD error = GetLastError();
		}
	}

	if (!succeeded) {
		return NULL; 
	}

	// Okay, HDC is setup to the rihgt format, now create our GL context

	// First, options for creating a debug context (potentially slower, but 
	// driver may report more useful errors). 
	int context_flags = 0; 
#if defined(_DEBUG)
	context_flags |= WGL_CONTEXT_DEBUG_BIT_ARB; 
#endif

	// describe the context
	int const attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,                             // Major GL Version
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,                             // Minor GL Version
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,   // Restrict to core (no compatibility)
		WGL_CONTEXT_FLAGS_ARB, context_flags,                             // Misc flags (used for debug above)
		0, 0
	};

	// Try to create context
	HGLRC context = wglCreateContextAttribsARB( hdc, NULL, attribs );
	if (context == NULL) {
		return NULL; 
	}

	return context;
}

//-----------------------------------------------------------------------------------------------
// Starts up the rendering system
//
void RenderingSystemStartup()
{
	// Initialize contexts and init functions
	RenderContextStartup();

	// Create the instance and complete post startup
	Renderer* rend = Renderer::CreateInstance();
	rend->PostStartup();
}

//-----------------------------------------------------------------------------------------------
// Shuts down the rendering system
//
void RenderingSystemShutdown()
{
	Renderer::DestroyInstance();

	// Terminate GL Contexts
	GLShutdown();
}

//-----------------------------------------------------------------------------------------------
// Rendering startup - called after we have created our window error checking 
//
bool RenderContextStartup() 
{
	// load and get a handle to the opengl dll (dynamic link library)
	g_GLLibrary = ::LoadLibraryA( "opengl32.dll" ); 
	g_displayDeviceContext = ::GetDC( (HWND) Window::GetInstance()->GetHandle());

	// use the DC to create a rendering context (handle for all OpenGL state - like a pointer)
	// This should be very similar to SD1
	HGLRC temp_context = CreateOldRenderContext( (HDC) g_displayDeviceContext ); 

	::wglMakeCurrent( (HDC) g_displayDeviceContext, temp_context ); 
	BindNewGLFunctions();  // find the functions we'll need to create the real context; 

						   // create the real context, using opengl version 4.2
	HGLRC real_context = CreateRealRenderContext( (HDC) g_displayDeviceContext, 4, 2 ); 

	// Set and cleanup
	::wglMakeCurrent( (HDC) g_displayDeviceContext, real_context ); 
	::wglDeleteContext( temp_context ); 

	// Bind all our OpenGL functions we'll be using.
	BindGLFunctions(); 

	// set the globals
	g_openGLRenderingContext = real_context; 

	return true; 
}

//-----------------------------------------------------------------------------------------------
// Shuts down OpenGL and clears contexts
//
void GLShutdown()
{
	wglMakeCurrent( (HDC) g_displayDeviceContext, NULL ); 

	::wglDeleteContext( (HGLRC) g_openGLRenderingContext ); 
	::ReleaseDC( (HWND) Window::GetInstance()->GetHandle(), (HDC) g_displayDeviceContext ); 

	g_openGLRenderingContext = NULL; 
	g_displayDeviceContext = NULL;
	

	::FreeLibrary( (HMODULE) g_GLLibrary ); 
}
