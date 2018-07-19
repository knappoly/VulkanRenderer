#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
struct RaycastHit3D
{
	bool	hit = false;
	Vector3 position;
	Vector3 normal;
};

typedef RaycastHit3D RaycastHit; // Assuming default raycast hit for engine