#pragma once
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/AABB2.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class TextureCube;
class VKFramebuffer;
class Material;
class Transform;
class VKRenderer;
class VKTexture;
class VKUniformBuffer;

//-----------------------------------------------------------------------------------------------
class VKCamera
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKCamera( VKRenderer* renderer );
	~VKCamera();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			Vector3			GetForward() const;
			Vector3			GetRight() const;
			Vector3			GetUp() const;
			void*			GetFrameBufferHandle();
			void			LookAt( const Vector3& position, const Vector3& target, const Vector3& up = Vector3::UP ); 
			void			SetProjectionMatrix( const Matrix44& projMatrix ); 
			void			SetViewMatrix( const Matrix44& viewMatrix );
			void			SetProjectionOrtho( float size, float near, float far ); 
			void			SetOrtho( float left, float right, float bottom, float top, float zNear, float zFar );
			void			SetPerspective( float fovDegrees, float aspect, float zNear, float zFar );
			void			SetColorTarget( VKTexture* colorTarget );
			void			SetDepthTarget( VKTexture* depthTarget );
			VKTexture*		GetColorTarget() const;
			VKTexture*		GetDepthTarget() const;
			void			SetSkyBox( const TextureCube* cubemap ) { m_skybox = cubemap; m_usesSkybox = true; }
			void			SetMaterial( Material* mat );
			void			SetViewport( const Vector2& mins, const Vector2& maxs );
			void			SetViewport( const AABB2& extent );
			Vector2			GetViewportMins() const { return m_viewport.mins; }
			Vector2			GetViewportMaxs() const { return m_viewport.maxs; }
			AABB2			GetViewportExtents() const { return m_viewport; }
			bool			IsSkyBoxValid() const { return m_usesSkybox; }
	const	TextureCube*	GetSkyBoxTexture() const { return m_skybox; }
			void*			GetRenderPass() const; 
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void			Translate( const Vector3& offset );
			void			RotateByEuler( const Vector3& rotation );
			void			Finalize(); // Internally calls finalize on the framebuffer
			void			UpdateMatrices();
	
	//-----------------------------------------------------------------------------------------------
	// Members
			AABB2				m_viewport;
			VKRenderer*			m_renderer = nullptr;
			Matrix44			m_viewMatrix;
			Matrix44			m_projMatrix;
			VKFramebuffer*		m_frameBuffer = nullptr;
			Transform*			m_transform = nullptr;
	const	TextureCube*		m_skybox = nullptr;
			bool				m_usesSkybox = false;
			Material*			m_material = nullptr;
			VKUniformBuffer*	m_cameraUBO = nullptr;
};

