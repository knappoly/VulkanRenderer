#include "Engine/VulkanRenderer/VKCamera.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Material.hpp"
#include "Engine/VulkanRenderer/VKFramebuffer.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/VulkanRenderer/VKTexture.hpp"
#include "Engine/VulkanRenderer/Buffers/VKUniformBuffer.hpp"
#include "Engine/Structures/UniformStructures.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
VKCamera::VKCamera(VKRenderer* renderer)
{
	m_renderer = renderer;
	m_frameBuffer = new VKFramebuffer(m_renderer);
	m_transform = new Transform();
	m_cameraUBO = new VKUniformBuffer(renderer);
	m_cameraUBO->Set<CameraBlock>(CameraBlock());
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKCamera::~VKCamera()
{
	delete m_frameBuffer;
	m_frameBuffer = nullptr;

	delete m_transform;
	m_transform = nullptr;

	if(m_material->m_shaderInstance)
	{
		delete m_material;
	}

	m_material = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Returns the forward 
//
Vector3 VKCamera::GetForward() const
{
	return m_transform->GetForward();
}

//-----------------------------------------------------------------------------------------------
// Returns the right 
//
Vector3 VKCamera::GetRight() const
{
	return m_transform->GetRight();
}

//-----------------------------------------------------------------------------------------------
// Returns the up  
//
Vector3 VKCamera::GetUp() const
{
	return m_transform->GetUp();
}

//-----------------------------------------------------------------------------------------------
// Returns the framebuffer handle 
//
void* VKCamera::GetFrameBufferHandle()
{
	return m_frameBuffer->GetHandle();
}

//-----------------------------------------------------------------------------------------------
// Looks at the target from the given position and Up vector
//
void VKCamera::LookAt(const Vector3& position, const Vector3& target, const Vector3& up /*= Vector3::UP */)
{
	Matrix44 camera = Matrix44::LookAt(position, target, up);
	m_transform->SetLocalMatrix(camera);

	SetViewMatrix(Matrix44::InvertFast(m_transform->GetWorldMatrix()));
}

//-----------------------------------------------------------------------------------------------
// Sets the projection matrix
//
void VKCamera::SetProjectionMatrix(const Matrix44& projMatrix)
{
	m_projMatrix = projMatrix;

	CameraBlock* block = m_cameraUBO->As<CameraBlock>();
	block->projection = m_projMatrix;
}

//-----------------------------------------------------------------------------------------------
// Sets the view matrix
//
void VKCamera::SetViewMatrix(const Matrix44& viewMatrix)
{
	m_viewMatrix = viewMatrix;

	CameraBlock* block = m_cameraUBO->As<CameraBlock>();
	block->view = m_viewMatrix;
}

//-----------------------------------------------------------------------------------------------
// Sets the projection matrix as an ortho matrix 
//
void VKCamera::SetProjectionOrtho(float size, float near, float far)
{
	float aspect = (float) m_frameBuffer->GetWidth() / (float) m_frameBuffer->GetHeight();
	float horizontal = size * aspect * 0.5f;
	float vertical = size * 0.5f;
	
	SetProjectionMatrix(Matrix44::MakeOrtho3D(-horizontal, horizontal, -vertical, vertical, near, far));
}

//-----------------------------------------------------------------------------------------------
// Sets the ortho matrix 
//
void VKCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	SetProjectionMatrix(Matrix44::MakeOrtho3D(left, right, bottom, top, zNear, zFar));
}

//-----------------------------------------------------------------------------------------------
// Sets the projection matrix as a perspective matrix
//
void VKCamera::SetPerspective(float fovDegrees, float aspect, float zNear, float zFar)
{
	SetProjectionMatrix(Matrix44::MakePerspectiveMatrix(fovDegrees, aspect, zNear, zFar));
}

//-----------------------------------------------------------------------------------------------
// Sets the color target on the framebuffer
//
void VKCamera::SetColorTarget(VKTexture* colorTarget)
{
	m_frameBuffer->SetColorTarget(colorTarget);
	m_viewport.maxs = Vector2(colorTarget->GetDimensions());
}

//-----------------------------------------------------------------------------------------------
// Sets the depth target on the framebuffer
//
void VKCamera::SetDepthTarget(VKTexture* depthTarget)
{
	m_frameBuffer->SetDepthStencilTarget(depthTarget);
	m_viewport.maxs = Vector2(depthTarget->GetDimensions());
}

//-----------------------------------------------------------------------------------------------
// Returns the colortarget
//
VKTexture* VKCamera::GetColorTarget() const
{
	return m_frameBuffer->GetColorTarget();
}

//-----------------------------------------------------------------------------------------------
// Returns the depth target
//
VKTexture* VKCamera::GetDepthTarget() const
{
	return m_frameBuffer->GetDepthTarget();
}

//-----------------------------------------------------------------------------------------------
// Sets the viewport rectangle
//
void VKCamera::SetViewport(const Vector2& mins, const Vector2& maxs)
{
	SetViewport(AABB2(mins,maxs));
}

//-----------------------------------------------------------------------------------------------
// Sets the viewport rectangle
//
void VKCamera::SetViewport(const AABB2& extent)
{
	m_viewport = extent;
}

//-----------------------------------------------------------------------------------------------
// Returns the render pass from the framebuffer
//
void* VKCamera::GetRenderPass() const
{
	return m_frameBuffer->GetRenderPass();
}

//-----------------------------------------------------------------------------------------------
// Translates the camera
//
void VKCamera::Translate(const Vector3& offset)
{
	m_transform->Translate(offset);

	Matrix44 temp = m_transform->GetWorldMatrix();
	SetViewMatrix(Matrix44::Invert(temp));
}

//-----------------------------------------------------------------------------------------------
// Rotates the camera by the given euler angles
//
void VKCamera::RotateByEuler(const Vector3& rotation)
{
	m_transform->RotateByEuler(rotation);
	Matrix44 temp = m_transform->GetWorldMatrix();
	SetViewMatrix(Matrix44::Invert(temp));
}

//-----------------------------------------------------------------------------------------------
// Finalizes the framebuffer
//
void VKCamera::Finalize()
{
	m_frameBuffer->Finalize();
}

//-----------------------------------------------------------------------------------------------
// Updates the matrices
//
void VKCamera::UpdateMatrices()
{
	Matrix44 temp = m_transform->GetWorldMatrix();
	SetViewMatrix(Matrix44::Invert(temp));
}
