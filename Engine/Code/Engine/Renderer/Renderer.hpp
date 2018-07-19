#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"
#include <string>
#include <vector>
#include <map>
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Structures/LightStructure.hpp"
#include "Engine/Structures/UniformStructures.hpp"
#include "Engine/Enumerations/TextureSlot.hpp"
#include "Engine/Enumerations/DrawPrimitiveType.hpp"
#include "Engine/Enumerations/ReservedUniformBlock.hpp"
#include "Engine/Structures/TextAlignment.hpp"
#include "Engine/Enumerations/WrapMode.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Renderer/FogBlock.hpp"

//-----------------------------------------------------------------------------------------------
// Constants

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class BitmapFont;
struct Vertex_3DPCU;
class ShaderProgram;
class RenderBuffer;
class Sampler;
class Image;
class Camera;
class FrameBuffer;
class Command;
class Mesh;
class Sprite;
class Texture;
class TextureCube;
class Shader;
class UniformBuffer;
class Light;
class Material;
struct VertexLayout;
struct RenderState;
enum eTextureFormat;
enum RenderType;
enum BlendFactor;
enum BlendOp;
enum DepthTestOp;
enum FillMode;
enum CullMode;
enum WindOrder;

//-----------------------------------------------------------------------------------------------
// OpenGL Handles
extern void* g_displayDeviceContext;			
extern void* g_openGLRenderingContext;		
extern void* g_GLLibrary;

//-----------------------------------------------------------------------------------------------
class Renderer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Renderer();
	~Renderer(){}

	//-----------------------------------------------------------------------------------------------
	// Member Functions
			void			BeginFrame();
			void			EndFrame();
			void			PostStartup();
	static	Renderer*		GetInstance();
	static	Renderer*		CreateInstance();
	static  void			DestroyInstance();

	//-----------------------------------------------------------------------------------------------
	// Command Callbacks
	static	bool			ScreenshotCommand(Command& cmd);

	//-----------------------------------------------------------------------------------------------
	// Draw Functions
	void			DrawLine2D( const Vector2& start, const Vector2& end, const Rgba& lineColor=Rgba::WHITE, float lineThickness=1.5f );
	void			DrawLine2D( const Vector2& start, const Vector2& end, const Rgba& startColor, const Rgba& endColor, float lineThickness  );
	void			DrawLine3D( const Vector3& start, const Vector3& end, const Rgba& lineColor = Rgba::WHITE );
	
	void			DrawLineLoop2D( const Vector2 vertices[], int numVertices, float lineThickness = 1.5f, const Rgba& objectColor = Rgba::WHITE );
	void			DrawLineLoop2D( const Vector2 vertices[], int numVertices, float lineThickness, const Rgba vertexColors[] );
	
	void			DrawTriangle2D( const Vector2 vertices[], float lineThickness = 1.5f, const Rgba& objectColor = Rgba::WHITE );
	void			DrawTriangle2D( const Vector2 vertices[], float lineThickness, const Rgba vertexColors[] );
	
	void			DrawCircle2D( float radius, float lineThickness = 1.5f, const Rgba& objectColor = Rgba::WHITE );
	
	void			DrawDottedCircle2D(float radius, float lineThickness = 1.5f, const Rgba& objectColor = Rgba::WHITE);
	
	void			DrawAABB2( const AABB2& bounds, const Rgba& color = Rgba::WHITE );
	void			DrawTexturedAABB2( const AABB2& bounds, const Texture& texture, const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs, const Rgba& tint = Rgba::WHITE);

	void			DrawText2D( const Vector2& drawMins, const std::string& asciiText, float cellHeight, const BitmapFont* font, const Rgba& tint = Rgba::WHITE, float aspectScale = 1.f );
	void			DrawTextInBox2D( const AABB2& textBoxBounds, const std::string& asciiText, float cellHeight, const Vector2& textAlignment, WrapMode wrapMode, const BitmapFont* font, const Rgba& tint = Rgba::WHITE, float aspectScale = 1.f );
	void			DrawText3D( const std::string& text, const Matrix44& transform,  const BitmapFont* font, const Rgba& tint = Rgba::WHITE );

	void			DrawCube( const Vector3& center, const Vector3& dimensions, const Rgba& color, const AABB2& uvTop = AABB2::ZERO_TO_ONE, const AABB2& uvSide = AABB2::ZERO_TO_ONE, const AABB2& uvBottom = AABB2::ZERO_TO_ONE);

	void			DrawSprite( const Sprite& sprite, const Matrix44& transform );

	void			DrawMeshImmediate( const Vertex_3DPCU* vertices, int numVerts, DrawPrimitiveType mode, const Matrix44& modelMatrix = Matrix44::IDENTITY );
	void			DrawMeshImmediateWithIndices( const Vertex_3DPCU* vertices, int numVerts, const unsigned int* indices, int numIndices, DrawPrimitiveType mode, const Matrix44& modelMatrix = Matrix44::IDENTITY );
	void			DrawMesh( Mesh* mesh, const Matrix44& modelMatrix = Matrix44::IDENTITY );

	//-----------------------------------------------------------------------------------------------
	// Debug Stuff
	void			SetRenderMode( int mode ) { m_renderMode = mode; }
	
	//-----------------------------------------------------------------------------------------------
	// Mesh functions
	Mesh*			CreateOrGetMesh( const std::string& path );
	void			InitializeDefaultMeshes();

	//-----------------------------------------------------------------------------------------------
	// Texture Functions
	void			SetTexture( Texture* texture );
	void			SetTexture( unsigned int index, Texture* texture, Sampler* sampler = nullptr );
	Texture*		CreateOrGetTexture(const std::string& path, bool genMipmaps = true);
	Texture*		CreateOrGetTexture(const Image& image, bool genMipmaps = true);
	bool			IsTextureLoaded(const std::string& path) const;
	void			BindTexture2D( const Texture* texture );
	void			BindTexture2D( unsigned int index, const Texture* texture );
	void			SetDefaultTexture();
	void			CopyTexture2D( Texture* dest, Texture* src );
	Texture*		CreateRenderTarget(unsigned int width, unsigned int height, eTextureFormat fmt = TEXTURE_FORMAT_RGBA8);
	Texture*		CreateDepthStencilTarget( unsigned int width, unsigned int height );

	//-----------------------------------------------------------------------------------------------
	// Cubemap functions
	TextureCube*	CreateOrGetCubeMap( const std::string& path );
	void			BindCubemap( const TextureCube* cubemap );

	//-----------------------------------------------------------------------------------------------
	// GIF Functions
	GIFAnimation*	AcquireGIFInstance( const std::string& path );

	//-----------------------------------------------------------------------------------------------
	// Bitmap Font functions
	BitmapFont*		CreateOrGetBitmapFont( const char* bitmapFontName );
	bool			IsBitmapFontLoaded( const std::string& bitmapFontName ) const;

	//-----------------------------------------------------------------------------------------------
	// Shader functions
	void			SetShader( Shader* shader = nullptr );
	ShaderProgram*	CreateOrGetShaderProgram(const std::string& path, const char* defines = nullptr);
	void			UseShaderProgram(const ShaderProgram* shaderProgram);
	void			BindDefaultShader();
	void			BindMeshToProgram( unsigned int programHandle, const Mesh* mesh );
	void			BindRenderState( RenderState state );
	void			BlendFunction(BlendFactor sfactor, BlendFactor dfactor );
	void			ColorBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
	void			SetDepthTestMode(DepthTestOp mode, bool flag);
	void			DisableDepth();
	void			EnableBlend() const;
	void			EnableLineSmooth() const;
	void			SetFillMode(FillMode fillMode);
	void			SetCullMode(CullMode cullMode);
	void			ReloadShaderPrograms();

	//-----------------------------------------------------------------------------------------------
	// Material Functions
	void			BindMaterial( const Material* material = nullptr );
	void			SetMaterial( const Material* material = nullptr );
	Material*		CreateOrGetMaterial( const std::string& path );
	void			SetDefaultMaterial();
	void			ResetDefaultMaterial();

	//-----------------------------------------------------------------------------------------------
	// Lighting methods
	void			SetAmbientLight( const Rgba& color = Rgba::WHITE, float intensity = 1.f );
	void			DisableAmbientLight();
	void			IncreaseAmbientLightIntensity( float amount );
	void			DecreaseAmbientLightIntensity( float amount );
	void			EnablePointLight( int index, const Vector3& position, const Rgba& color = Rgba::WHITE, float intensity = 1.f, const Vector3& attenuation = Vector3(0.f, 1.f, 0.f) );
	void			EnableDirectionalLight( int index, const Vector3& position, const Vector3& dir, const Rgba& color = Rgba::WHITE, float intensity = 1.f );
	void			EnableSpotlight( int index, const Vector3& position, const Vector3& dir, float innerAngle, float outerAngle, const Rgba& color = Rgba::WHITE, float intensity = 1.f );
	void			SetLightPosition( int index, const Vector3& position );
	void			SetLightDirection( int index, const Vector3& dir );
	void			DisableAllLights();
	void			DisableLight( int index );
	void			SetSpecularFactor( float factor );
	void			SetSpecularPower( float power );
	void			SetLightBuffer( std::vector<Light*>& lights );

	//-----------------------------------------------------------------------------------------------
	// Setting uniforms on shaders
	void			BindUBO( int bindPoint, const UniformBuffer* ubo );
	void			SetUniform( const char* name, float value );
	void			SetUniform( const char* name, int value );
	void			SetUniform( const char* name, const Rgba& color );
	void			SetUniform( const char* name, const Matrix44& matrix, bool transpose = false );
	void			SetUniform( const char* name, const Vector3& value );

	//-----------------------------------------------------------------------------------------------
	// Image effects functions
	void			ApplyEffect( const ShaderProgram* shaderProgram );
	void			FinishEffects();

	//-----------------------------------------------------------------------------------------------
	// GL Enum getters
	unsigned int	GetGLPrimitive(DrawPrimitiveType mode) const;
	unsigned int	GetGLDepthTestMode(DepthTestOp mode) const;
	unsigned int	GetGLDataType(RenderType type) const;
	unsigned int	GetGLBlendOp(BlendOp mode) const;
	unsigned int	GetGLBlendFactor(BlendFactor factor) const;
	unsigned int	GetGLWindOrder(WindOrder order) const;
	unsigned int	GetGLFillMode(FillMode mode) const;
	unsigned int	GetGLCullMode(CullMode mode) const;

	//-----------------------------------------------------------------------------------------------
	// OpenGL Wrappers and Projection functions
	void			SetOrtho(const Vector2& bottomLeft, const Vector2& topRight, float nearPoint, float farPoint);
	void			SetOrtho(float left, float right, float down, float up, float zNear, float zFar);
	void			SetOrtho(const Matrix44& projMatrix);
	void			SetProjectionOrtho(float size, float zNear, float zFar);
	void			SetPerspective( float fovDegrees, float aspect, float zNear, float zFar );
	void			SetViewMatrix(const Matrix44& viewMatrix);
	void			ClearScreen ( const Rgba& clearColor);
	void			ClearDepth( float depth = 1.f );
	void			LineWidth(float width) const;
	bool			CopyFrameBuffer( FrameBuffer *dst, FrameBuffer *src );
	void			SetCamera(Camera* cam);
	void			TakeScreenshot(const char* fileName);
	

	//-----------------------------------------------------------------------------------------------
	// Data
			std::map<std::string,Texture*>			m_loadedTextures; 
			std::map<std::string,TextureCube*>		m_loadedCubemaps;
			std::map<std::string,GIFAnimation*>		m_loadedGIFs;
			std::map<std::string,BitmapFont*>		m_loadedFonts;
			std::map<std::string,ShaderProgram*>	m_loadedShaderPrograms; 
			std::map<std::string, Mesh*>			m_loadedMeshes;
			std::map<std::string,Material*>			m_loadedMaterials;
			unsigned int							m_defaultVAO = NULL;
			Texture*								m_defaultTexture = nullptr;
			Camera*									m_defaultCamera = nullptr;
			Camera*									m_defaultPerspectiveCam = nullptr;
			Camera*									m_currentCamera = nullptr;
			Texture*								m_defaultColorTarget = nullptr;
			Texture*								m_defaultDepthTarget = nullptr;
			Shader*									m_defaultShader = nullptr;
			Material*								m_defaultMaterial = nullptr;
			Material*								m_defaultMaterialShared = nullptr;
			Material*								m_activeMaterial = nullptr;
			LightBlock*								m_lightBlock = nullptr;
			SpecularBlock*							m_specularBlock = nullptr;
			UniformBuffer*							m_lightBuffer = nullptr;
			UniformBuffer*							m_specularBuffer = nullptr;
			UniformBuffer*							m_cameraBuffer = nullptr;
			UniformBuffer*							m_fogBuffer = nullptr;
			int										m_renderMode = 0;

	private:	
			Camera*									m_effectCamera = nullptr;
			Texture*								m_effectTarget = nullptr;
			Texture*								m_effectScratch = nullptr;
			FrameBuffer*							m_copyDestination = nullptr;
			FrameBuffer*							m_copySrc = nullptr;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
void RenderingSystemStartup();
void RenderingSystemShutdown();
bool RenderContextStartup();
void GLShutdown();