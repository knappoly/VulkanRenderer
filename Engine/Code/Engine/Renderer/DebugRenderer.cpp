#include "Engine/Renderer/DebugRenderer.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/EngineConfig.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/CommandDefinition.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Static Globals
static DebugRenderer* g_debugRenderer = nullptr;
std::vector<DebugRenderObject*>	DebugRenderer::s_debugObjects;
std::vector<DebugRenderObject*>	DebugRenderer::s_debugLogBuffer;

//-----------------------------------------------------------------------------------------------
// Destructor
//
DebugRenderer::~DebugRenderer()
{
	delete m_debug2DCamera;
	m_debug2DCamera = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Command call to clear all draw renders
//
bool DebugRenderer::ClearCommand(Command& cmd)
{
	UNUSED(cmd);
	g_debugRenderer->ClearDebugRenders();

	return true;
}

//-----------------------------------------------------------------------------------------------
// Command callback to enable or disable debug render display
//
bool DebugRenderer::ShowDebugCommand(Command& cmd)
{
	int flag = 1;
	cmd.GetNextInt(flag);
	if(flag)
	{
		g_debugRenderer->EnableDebugRender();
	}
	else
	{
		g_debugRenderer->DisableDebugRender();
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
// Creates a debug renderer instance
//
STATIC DebugRenderer* DebugRenderer::CreateInstance()
{
	if(g_debugRenderer == nullptr)
	{
		g_debugRenderer = new DebugRenderer();
	}

	return g_debugRenderer;
}

//-----------------------------------------------------------------------------------------------
// Returns the debug renderer instance
//
STATIC DebugRenderer* DebugRenderer::GetInstance()
{
	return g_debugRenderer;
}

//-----------------------------------------------------------------------------------------------
// Destroys the debug renderer instance
//
STATIC void DebugRenderer::DestroyInstance()
{
	if(g_debugRenderer)
	{
		g_debugRenderer->Shutdown();
		delete g_debugRenderer;
		g_debugRenderer = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Starts up the debug renderer
//
void DebugRenderer::Startup()
{
	// Setup the 2D camera
	m_debug2DCamera = new Camera();
	m_debug2DCamera->SetColorTarget(Renderer::GetInstance()->m_defaultColorTarget);

	// Setup projection on the 2D camera
	Window* window = Window::GetInstance();
	m_debug2DCamera->SetOrtho(0.f, window->m_width, 0.f, window->m_height, 0.f, 1.f);
	
	COMMAND("showdebug", ShowDebugCommand, "Enables/Disables debug draws");
	COMMAND("cleardebug", ClearCommand, "Clears all debug draws");
}

//-----------------------------------------------------------------------------------------------
// Shuts down the debug renderer
//
void DebugRenderer::Shutdown()
{
	g_debugRenderer->ClearDebugRenders();
}

//-----------------------------------------------------------------------------------------------
// Clears all debug renders
//
void DebugRenderer::ClearDebugRenders()
{
	s_debugObjects.clear();
	s_debugLogBuffer.clear();
}

//-----------------------------------------------------------------------------------------------
// Updates the debug render objects
//
void DebugRenderer::Update(float deltaSeconds)
{
	RemoveFinishedDebugObjects();
	RemoveFinishedLogTexts();

	for( DebugRenderObject* debug : s_debugObjects )
	{
		debug->Update(deltaSeconds);
	}

	for( DebugRenderObject* logText : s_debugLogBuffer )
	{
		logText->Update(deltaSeconds);
	}

	
}

//-----------------------------------------------------------------------------------------------
// Renders the debug render objects
//
void DebugRenderer::Render() const
{
	if(IsDebugEnabled())
	{
		for (DebugRenderObject* debug : s_debugObjects)
		{
			debug->Render();
		}

		RenderLog();
	}
}

//-----------------------------------------------------------------------------------------------
// Renders the debug log on the screen
//
void DebugRenderer::RenderLog() const
{
	Window* window = Window::GetInstance();
	// Font size as a ratio of the screen height
	float fontSize = window->m_height * m_logTextScreenRatio;

	Vector2 drawMins;
	drawMins.y = window->m_height * 0.99f;
	drawMins.x = window->m_width * 0.05f;
	for (DebugRenderObject* debug : s_debugLogBuffer)
	{
		drawMins.y -= fontSize;
		debug->m_transform->SetPosition(drawMins);
		debug->Render();
	}
}

//-----------------------------------------------------------------------------------------------
// Removes the debug render object from the list
//
void DebugRenderer::RemoveFinishedDebugObjects()
{
	for( size_t index = 0; index < s_debugObjects.size(); ++index )
	{
		if(s_debugObjects[index]->IsFinished())
		{
			delete s_debugObjects[index];
			s_debugObjects[index] = s_debugObjects[s_debugObjects.size() - 1];
			s_debugObjects.pop_back();
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Removes text from the log over time. Preserves order while erasing
//
void DebugRenderer::RemoveFinishedLogTexts()
{
	for( size_t index = 0; index < s_debugLogBuffer.size(); ++index )
	{
		if(s_debugLogBuffer[index]->IsFinished())
		{
			delete s_debugLogBuffer[index];
			s_debugLogBuffer.erase(s_debugLogBuffer.begin() + index);
			index--;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Draws a point on the screen
//
void DebugRenderer::DebugRenderPoint(const Vector3& position, const DebugRenderOptions& options)
{
	Mesh* mesh = CreatePoint(position);
	
	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a quad on the screen
//
void DebugRenderer::DebugRenderQuad( const Vector3& position, const Vector3& euler, const Vector2& scale, const DebugRenderOptions& options, bool isFacingCamera /*= false*/, const Texture* texture /*= nullptr*/)
{
	Mesh* mesh = CreateQuad(Vector2(-0.5f, -0.5f), Vector2(0.5f, 0.5f));

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);
	debugObj->SetTexture(texture);
	debugObj->OrientToCamera(isFacingCamera);
	debugObj->m_shader->SetCullMode(CULLMODE_NONE);
	debugObj->m_transform->SetPosition(position);
	debugObj->m_transform->SetEulerAngles(euler);
	debugObj->m_transform->SetScale(scale);
	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a line segment on the screen
//
void DebugRenderer::DebugRenderLineSegment(const Vector3& start, const Vector3& end, const DebugRenderOptions& options)
{
	Mesh* mesh = CreateLine(start, end);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a sphere on the screen 
//
void DebugRenderer::DebugRenderSphere(const Vector3& origin, float radius, uint wedges, uint slices, FillMode fillMode, const DebugRenderOptions& options, 
	const Texture* texture /*= nullptr*/)
{
	Mesh* mesh = CreateUVSphere(origin, radius, wedges, slices);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);
	debugObj->SetFillMode(fillMode);
	debugObj->SetTexture(texture);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a AABB3 on the screen
//
void DebugRenderer::DebugRenderAABB3(const Vector3& position, const Vector3& size, FillMode fillMode, const DebugRenderOptions& options, const Texture* texture /*= nullptr */)
{
	Mesh* mesh = CreateCube(position, size);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);
	debugObj->SetFillMode(fillMode);
	debugObj->SetTexture(texture);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a basis on the screen
//
void DebugRenderer::DebugRenderBasis(const Matrix44& basis, float scale, const DebugRenderOptions& options)
{
	Mesh* mesh = CreateBasisMesh(basis, scale);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a grid in the scene
//
void DebugRenderer::DebugRenderGrid(const Vector3& right, const Vector3& up, int uniformSize, const DebugRenderOptions& options)
{
	Mesh* mesh = CreateGridMesh(right, up, uniformSize);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws 3D text on the screen 
//
void DebugRenderer::DebugRender3DText(const Vector3& position, const Vector3& euler, const std::string& text, float cellHeight, const DebugRenderOptions& options, 
									const Vector2& alignment, bool isFacingCamera /*= false */)
{
	BitmapFont* font = Renderer::GetInstance()->CreateOrGetBitmapFont("SquirrelFixedFont");
	Mesh* mesh = CreateTextMesh2D(Vector3::ZERO, text, cellHeight, font, Rgba::WHITE, alignment);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug3DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);
	debugObj->OrientToCamera(isFacingCamera);
	debugObj->SetTexture(font->GetSpriteSheetTexture());
	debugObj->m_transform->SetPosition(position);
	debugObj->m_transform->SetEulerAngles(euler);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a 2D quad on the screen
//
void DebugRenderer::DebugRender2DQuad(const AABB2& bounds, const DebugRenderOptions& options)
{
	Mesh* mesh = CreateQuad(bounds.mins, bounds.maxs);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug2DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws a 2D line on the screen
//
void DebugRenderer::DebugRender2DLine(const Vector2& start, const Vector2& end, const Rgba& startVertColor, const Rgba& endVertColor, const DebugRenderOptions& options)
{
	Mesh* mesh = CreateLine(start, end, startVertColor, endVertColor);

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug2DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws 2d text on the screen
//
void DebugRenderer::DebugRender2DText(const Vector2& position, const std::string& text, float cellHeight, const DebugRenderOptions& options, const Vector2& alignment)
{
	BitmapFont* font = Renderer::GetInstance()->CreateOrGetBitmapFont("SquirrelFixedFont");
	Mesh* mesh = CreateTextMesh2D(position, text, cellHeight, font, Rgba::WHITE, alignment);  // Color set by shader

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug2DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);
	debugObj->SetTexture(font->GetSpriteSheetTexture());

	s_debugObjects.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Draws log text on the screen
//
void DebugRenderer::DebugRenderLog(const std::string& text, const DebugRenderOptions& options)
{
	BitmapFont* font = Renderer::GetInstance()->CreateOrGetBitmapFont("SquirrelFixedFont");
	float fontSize = Window::GetInstance()->m_height * m_logTextScreenRatio;
	Mesh* mesh = CreateTextMesh2D(Vector3::ZERO, text, fontSize, font);  // Color set by shader

	DebugRenderObject* debugObj = new DebugRenderObject();
	debugObj->SetDebugCamera(m_debug2DCamera);
	debugObj->SetMesh(mesh);
	debugObj->SetRenderOptions(options);
	debugObj->SetTexture(font->GetSpriteSheetTexture());

	s_debugLogBuffer.push_back(debugObj);
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
DebugRenderObject::DebugRenderObject()
{
	m_shader = Shader::AcquireResource("Data/Shaders/debug.shader");
	m_transform = new Transform();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
DebugRenderObject::~DebugRenderObject()
{
	delete m_transform;
	m_transform = nullptr;

	delete m_shader;
	m_shader = nullptr;

	delete m_mesh;
	m_mesh = nullptr;

}

//-----------------------------------------------------------------------------------------------
// Sets the debug render options
//
void DebugRenderObject::SetRenderOptions(const Rgba& startColor, const Rgba& endColor, float lifeTime, DebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */)
{
	m_options.startColor = startColor;
	m_options.endColor = endColor;
	m_options.lifetime = lifeTime;
	m_options.mode = mode;
}

//-----------------------------------------------------------------------------------------------
// Sets the fill mode on the shader 
//
void DebugRenderObject::SetFillMode(FillMode fillMode)
{
	m_shader->SetFillMode(fillMode);
}

//-----------------------------------------------------------------------------------------------
// Updates the render object
//
void DebugRenderObject::Update(float deltaSeconds)
{
	m_elapsedSeconds += deltaSeconds;
	float fractionTowardsEnd = m_options.lifetime == 0.f ? 1.f : m_elapsedSeconds * 1.f / m_options.lifetime;

	if(m_elapsedSeconds >= m_options.lifetime)
	{
		m_isFinished = true;
		fractionTowardsEnd = 1.f;
	}

	if(m_isCameraOriented)
	{
		Vector3 euler = m_camera->m_transform.GetEulerAngles();
		m_transform->SetEulerAngles(euler);
	}

	m_currentColor = Interpolate(m_options.startColor, m_options.endColor, fractionTowardsEnd);
}

//-----------------------------------------------------------------------------------------------
// Renders the debug object
//
void DebugRenderObject::Render() const
{
	Renderer* rend = Renderer::GetInstance();
	rend->SetCamera(m_camera);
	rend->SetTexture((Texture*) m_texture);
	rend->SetShader(m_shader);
	rend->SetUniform("CURCOLOR", m_currentColor);
	rend->SetDefaultMaterial();

	if(m_options.mode == DEBUG_RENDER_IGNORE_DEPTH)
	{
		m_shader->SetDepthTest(COMPARE_ALWAYS, true);
	}
	else if(m_options.mode == DEBUG_RENDER_XRAY)
	{
		rend->SetUniform("CURCOLOR", Rgba(m_currentColor.r, m_currentColor.g, m_currentColor.b, 100));
		m_shader->SetDepthTest(COMPARE_GREATER, true);
		rend->DrawMesh(m_mesh, m_transform->GetWorldMatrix());
		rend->SetUniform("CURCOLOR", m_currentColor);
		m_shader->SetDepthTest(COMPARE_LESS, true);
	}
	else if(m_options.mode == DEBUG_RENDER_HIDDEN)
	{
		m_shader->SetDepthTest(COMPARE_GREATER, true);
	}
	else
	{
		m_shader->SetDepthTest(COMPARE_LESS, true);
	}
	rend->DrawMesh(m_mesh, m_transform->GetWorldMatrix());
}

//-----------------------------------------------------------------------------------------------
// Starts up the debug render system
//
void DebugRendererStartup()
{
	#if defined(ENGINE_ENABLE_DEBUG_RENDERING)
		DebugRenderer::CreateInstance();
		DebugRenderer::GetInstance()->Startup();
	#endif
}

//-----------------------------------------------------------------------------------------------
// Shuts down the debug render system
//
void DebugRendererShutdown()
{
	#if defined(ENGINE_ENABLE_DEBUG_RENDERING)
		DebugRenderer::DestroyInstance();
	#endif
}
