#include "Engine/Renderer/OrbitCamera.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
OrbitCamera::OrbitCamera():Camera()
{

}

//-----------------------------------------------------------------------------------------------
// Sets the target and sets the view matrix
//
void OrbitCamera::SetTarget(const Vector3& target)
{
	m_target = target;
}

//-----------------------------------------------------------------------------------------------
// Sets the spherical coordinates
//
void OrbitCamera::SetSphericalCoordinates(float radius, float rotation, float azimuth)
{
	m_radius = radius;
	m_rotation = rotation;
	m_azimuth = azimuth;

	LookAt(SphericalToCartesian(m_radius, m_rotation, m_azimuth) + m_target, m_target);
}
