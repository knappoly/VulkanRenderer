#pragma once
#include "Engine\Math\Transform.hpp"
#include "Engine\Math\AABB2.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;
class TextureCube;
class FrameBuffer;
class Material;

//-----------------------------------------------------------------------------------------------
class Camera 
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Camera();
	~Camera();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			Vector3			GetForward() const { return m_transform.GetForward(); }
			Vector3			GetRight() const { return m_transform.GetRight(); }
			Vector3			GetUp() const { return m_transform.GetUp(); }
			unsigned int	GetFrameBufferHandle();
			void			LookAt( const Vector3& position, const Vector3& target, const Vector3& up = Vector3::UP ); 
			void			SetProjection( const Matrix44& projMatrix ); 
			void			SetProjectionOrtho( float size, float near, float far ); 
			void			SetOrtho( float left, float right, float bottom, float top, float zNear, float zFar );
			void			SetPerspective( float fovDegrees, float aspect, float zNear, float zFar );
			void			SetColorTarget( Texture* colorTarget );
			void			SetDepthTarget( Texture* depthTarget );
			Texture*		GetDepthTarget() const;
			Texture*		GetColorTarget() const;
			void			SetSkyBox( const TextureCube* cubemap ) { m_skybox = cubemap; m_usesSkybox = true; }
			void			SetMaterial( Material* mat );
			void			SetViewport( const Vector2& mins, const Vector2& maxs );
			void			SetViewport( const AABB2& extent );
			Vector2			GetViewportMins() const { return m_viewport.mins; }
			Vector2			GetViewportMaxs() const { return m_viewport.maxs; }
			AABB2			GetViewportExtents() const { return m_viewport; }
			bool			IsSkyBoxValid() const { return m_usesSkybox; }
	const	TextureCube*	GetSkyBoxTexture() const { return m_skybox; }

	//-----------------------------------------------------------------------------------------------
	// Methods
			void			Translate( const Vector3& offset );
			void			RotateByEuler( const Vector3& rotation );
			void			Finalize(); // Internally calls finalize on the framebuffer
			void			UpdateMatrices();

	//-----------------------------------------------------------------------------------------------
	// Members
			Matrix44		m_viewMatrix;
			Matrix44		m_projMatrix;
			FrameBuffer*	m_frameBuffer = nullptr;
			Transform		m_transform;
	const	TextureCube*	m_skybox = nullptr;
			bool			m_usesSkybox = false;
			Material*		m_material = nullptr;
			AABB2			m_viewport;
};

