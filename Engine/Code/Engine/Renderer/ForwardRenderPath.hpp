#pragma once
#include <vector>
#define DEBUG_RENDER_LIGHTS

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Camera;
class RenderScene;
class DrawCall;
class Light;

//-----------------------------------------------------------------------------------------------
class ForwardRenderPath
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	ForwardRenderPath();
	~ForwardRenderPath();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void	Render( RenderScene* scene );
	void	RenderSceneForCamera( Camera* cam, RenderScene* scene );
	void	RenderShadowObjectForLight( Light* light, RenderScene* scene );
	void	SortDrawsBySortOrder( std::vector<DrawCall>& drawCalls );
	void	SortDrawsByRenderQueue( std::vector<DrawCall>& drawCalls );
	void	SortDrawsByCameraDistance( std::vector<DrawCall>& drawCalls, Camera* cam );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	Camera*	m_shadowCamera;
};

