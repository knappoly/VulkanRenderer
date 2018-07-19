#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Renderer/Texture.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
Camera::Camera()
{
	m_frameBuffer = new FrameBuffer();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Camera::~Camera()
{
	delete m_frameBuffer;
	m_frameBuffer = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Returns the handle to the framebuffer
//
unsigned int Camera::GetFrameBufferHandle()
{
	return m_frameBuffer->GetHandle();
}

//-----------------------------------------------------------------------------------------------
// Makes the camera look at a target
//
void Camera::LookAt(const Vector3& position, const Vector3& target, const Vector3& up /*= Vector3::UP */)
{
	Matrix44 camera = Matrix44::LookAt(position, target, up);
	m_transform.SetLocalMatrix(camera);

	m_viewMatrix = Matrix44::InvertFast(m_transform.GetWorldMatrix());
}

//-----------------------------------------------------------------------------------------------
// Sets the projection matrix
//
void Camera::SetProjection(const Matrix44& projMatrix)
{
	m_projMatrix = projMatrix;
}

//-----------------------------------------------------------------------------------------------
// Sets the projection matrix from width and height on the framebuffer
//
void Camera::SetProjectionOrtho(float size, float near, float far)
{
	float aspect = (float) m_frameBuffer->GetWidth()/ (float) m_frameBuffer->GetHeight();
	float horizontal = size * aspect * 0.5f;
	float vertical = size * 0.5f;
	m_projMatrix = Matrix44::MakeOrtho3D(-horizontal, horizontal, -vertical, vertical, near, far);
}

//-----------------------------------------------------------------------------------------------
// Sets the projection matrix with the given coordinates
//
void Camera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_projMatrix = Matrix44::MakeOrtho3D(left, right, bottom, top, zNear, zFar);
}

//-----------------------------------------------------------------------------------------------
// Sets the projection as perspective
//
void Camera::SetPerspective(float fovDegrees, float aspect, float zNear, float zFar)
{
	m_projMatrix = Matrix44::MakePerspectiveMatrix(fovDegrees, aspect, zNear, zFar);
}

//-----------------------------------------------------------------------------------------------
// Sets the color target on the framebuffer
//
void Camera::SetColorTarget(Texture* colorTarget)
{
	m_frameBuffer->SetColorTarget(colorTarget);
	m_viewport.maxs = Vector2(colorTarget->GetDimensions());
}
//-----------------------------------------------------------------------------------------------
// Sets the depth target on the framebuffer
//
void Camera::SetDepthTarget(Texture* depthTarget)
{
	m_frameBuffer->SetDepthStencilTarget(depthTarget);
	m_viewport.maxs = Vector2(depthTarget->GetDimensions());
}

//-----------------------------------------------------------------------------------------------
// Returns the depth target
//
Texture* Camera::GetDepthTarget() const
{
	return m_frameBuffer->m_depthStencilTarget;
}

//-----------------------------------------------------------------------------------------------
// Returns the color target
//
Texture* Camera::GetColorTarget() const
{
	return m_frameBuffer->m_colorTarget;
}

//-----------------------------------------------------------------------------------------------
// Sets the material on the camera
//
void Camera::SetMaterial(Material* mat)
{
	m_material = mat;
}

//-----------------------------------------------------------------------------------------------
// Sets the viewport extents
//
void Camera::SetViewport(const AABB2& extent)
{
	m_viewport = extent;
}

//-----------------------------------------------------------------------------------------------
// Sets the viewport extents
//
void Camera::SetViewport(const Vector2& mins, const Vector2& maxs)
{
	SetViewport(AABB2(mins,maxs));
}

//-----------------------------------------------------------------------------------------------
// Translates the camera in space
//
void Camera::Translate(const Vector3& offset)
{
	m_transform.Translate(offset);

	Matrix44 temp = m_transform.GetWorldMatrix();
	m_viewMatrix = Matrix44::InvertFast(temp);
}

//-----------------------------------------------------------------------------------------------
// Adds rotation to the camera
//
void Camera::RotateByEuler(const Vector3& rotation)
{
	m_transform.RotateByEuler(rotation);
	Matrix44 temp = m_transform.GetWorldMatrix();
	m_viewMatrix = Matrix44::InvertFast(temp);
}

//-----------------------------------------------------------------------------------------------
// Calls the finalize method on the framebuffer
//
void Camera::Finalize()
{
	m_frameBuffer->Finalize();
}

//-----------------------------------------------------------------------------------------------
// Updates the camera matrices 
//
void Camera::UpdateMatrices()
{
	Matrix44 temp = m_transform.GetWorldMatrix();
	m_viewMatrix = Matrix44::Invert(temp);
}
