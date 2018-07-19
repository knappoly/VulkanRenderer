#pragma once
#include <vector>
#include <functional>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Renderable;
class Light;
class Camera;
class Vector3;
typedef std::function<void(Camera*)> PrerenderCB;
typedef std::function<void()> PrerenderIndieCB;

//-----------------------------------------------------------------------------------------------
class RenderScene
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	RenderScene(){}
	~RenderScene(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	std::vector<Light*> GetMostContributingLights( const Vector3& position );
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void				AddRenderable( Renderable* r );
	void				AddLight( Light* light ); 
	void				AddCamera( Camera* cam ); 
	void				AddPreRender( PrerenderCB cb );
	void				AddPreRenderIndie( PrerenderIndieCB cb );
	void				RemoveRenderable( Renderable* r );
	void				RemoveLight( Light* light );
	void				RemoveCamera( Camera* cam );
	void				RemovePreRender( PrerenderCB cb );
	void				RemovePreRenderIndie( PrerenderIndieCB cb );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	std::vector<Renderable*>		m_renderables;
	std::vector<Light*>				m_lights;
	std::vector<Camera*>			m_cameras;
	std::vector<PrerenderCB>		m_prerenders;
	std::vector<PrerenderIndieCB>	m_indiePrerenders;
};

