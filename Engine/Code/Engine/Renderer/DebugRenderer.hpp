#pragma once
#include <vector>
#include <string>
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Mesh;
class Camera;
class Texture;
class Command;
class Shader;
class Transform;
enum FillMode;

typedef unsigned int uint;

//-----------------------------------------------------------------------------------------------
enum DebugRenderMode 
{
	DEBUG_RENDER_IGNORE_DEPTH, // will always draw and be visible 
	DEBUG_RENDER_USE_DEPTH,    // draw using normal depth rules
	DEBUG_RENDER_HIDDEN,       // only draws if it would be hidden by depth
	DEBUG_RENDER_XRAY         // always draws, but hidden area will be drawn differently
};

//-----------------------------------------------------------------------------------------------
struct DebugRenderOptions
{
	DebugRenderOptions()
		: startColor(Rgba::WHITE)
		, endColor(Rgba::WHITE)
		, lifetime(0.0f)
		, mode(DEBUG_RENDER_USE_DEPTH)
	{}

	Rgba				startColor; 
	Rgba				endColor; 
	float				lifetime; 
	DebugRenderMode		mode; 
}; 

//-----------------------------------------------------------------------------------------------
struct DebugRenderObject
{
	//-----------------------------------------------------------------------------------------------
	// Constructor
	DebugRenderObject();
	~DebugRenderObject();

	//-----------------------------------------------------------------------------------------------
	// Accessors / Mutators
	bool	IsFinished() const { return m_isFinished; }
	void	SetRenderOptions( const DebugRenderOptions& options ) { m_options = options; }
	void	SetRenderOptions( const Rgba& startColor, const Rgba& endColor, float lifeTime, DebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
	void	SetDebugCamera( Camera* cam ) { m_camera = cam; }
	void	SetMesh( Mesh* mesh ) { m_mesh = mesh; }
	void	SetFillMode( FillMode fillMode );
	void	SetTexture( const Texture* texture ) { m_texture = texture; }
	void	OrientToCamera( bool flag ) { m_isCameraOriented = flag; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void	Update( float deltaSeconds );
	void	Render() const;
	
	//-----------------------------------------------------------------------------------------------
	// Members
			Camera*				m_camera = nullptr;
			Mesh*				m_mesh = nullptr;
	const	Texture*			m_texture = nullptr;
			bool				m_isFinished = false;
			bool				m_isCameraOriented = false;
			float				m_elapsedSeconds = 0.f;
			Rgba				m_currentColor;
			DebugRenderOptions	m_options;
			Shader*				m_shader = nullptr;
			Transform*			m_transform = nullptr;
};

//-----------------------------------------------------------------------------------------------
class DebugRenderer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	DebugRenderer(){}
	~DebugRenderer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			bool	HasRenderables() const { return s_debugObjects.size() > 0; }
			bool	IsDebugEnabled() const { return m_isDebugEnabled; }
			void	SetDebug3DCamera( Camera* cam ) { m_debug3DCamera = cam; }
			void	SetLogTextScreenRatio( float ratio ) { m_logTextScreenRatio = ratio; }

	//-----------------------------------------------------------------------------------------------
	// Command Callbacks
	static	bool			ClearCommand(Command& cmd);
	static	bool			ShowDebugCommand(Command& cmd);

	//-----------------------------------------------------------------------------------------------
	// Methods
	static	DebugRenderer*	CreateInstance();
	static	DebugRenderer*	GetInstance();
	static	void			DestroyInstance();
			void			Startup();
			void			Shutdown();
			void			ClearDebugRenders();
			void			Update( float deltaSeconds );
			void			Render() const;
			void			RenderLog() const;
			void			RemoveFinishedDebugObjects();
			void			RemoveFinishedLogTexts( );
			void			EnableDebugRender() { m_isDebugEnabled = true; }
			void			DisableDebugRender() { m_isDebugEnabled = false; }

	//-----------------------------------------------------------------------------------------------
	// Debug Draw 3D Methods
			void			DebugRenderPoint( const Vector3& position, const DebugRenderOptions& options );
			void			DebugRenderQuad( const Vector3& position, const Vector3& euler, const Vector2& scale, const DebugRenderOptions& options, bool isFacingCamera = false, const Texture* texture = nullptr);
			void			DebugRenderLineSegment( const Vector3& start, const Vector3& end, const DebugRenderOptions& options );
			void			DebugRenderSphere( const Vector3& origin, float radius, uint wedges, uint slices, FillMode fillMode, const DebugRenderOptions& options, const Texture* texture = nullptr);
			void			DebugRenderAABB3( const Vector3& position, const Vector3& size, FillMode fillMode, const DebugRenderOptions& options, const Texture* texture = nullptr );
			void			DebugRenderBasis( const Matrix44& basis, float scale, const DebugRenderOptions& options );
			void			DebugRenderGrid( const Vector3& right , const Vector3& up, int uniformSize, const DebugRenderOptions& options );
			void			DebugRender3DText( const Vector3& position, const Vector3& euler, const std::string& text, float cellHeight, const DebugRenderOptions& options, const Vector2& alignment, bool isFacingCamera = false );

	//-----------------------------------------------------------------------------------------------
	// Debug Draw 2D Methods
			void			DebugRender2DQuad( const AABB2& bounds, const DebugRenderOptions& options );
			void			DebugRender2DLine( const Vector2& start, const Vector2& end, const Rgba& startVertColor, const Rgba& endVertColor, const DebugRenderOptions& options );
			void			DebugRender2DText( const Vector2& position, const std::string& text, float cellHeight, const DebugRenderOptions& options, const Vector2& alignment );
			void			DebugRenderLog( const std::string& text, const DebugRenderOptions& options );
	
	//-----------------------------------------------------------------------------------------------
	// Members
			bool			m_isDebugEnabled = true;
			Camera*			m_debug2DCamera = nullptr;
			Camera*			m_debug3DCamera = nullptr;
			float			m_logTextScreenRatio = 0.015f;
	
	//-----------------------------------------------------------------------------------------------
	// Static Members
	static	std::vector<DebugRenderObject*>	s_debugObjects;
	static	std::vector<DebugRenderObject*>	s_debugLogBuffer;
			
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
void	DebugRendererStartup();
void	DebugRendererShutdown();
