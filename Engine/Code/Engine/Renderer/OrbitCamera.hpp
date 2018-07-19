#pragma once
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class OrbitCamera: public Camera
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	OrbitCamera();
	~OrbitCamera(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void	SetTarget(const Vector3& target);
	void	SetSphericalCoordinates(float radius, float rotation, float azimuth);
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3 m_target;
	float	m_radius;
	float	m_rotation;
	float	m_azimuth;
};

