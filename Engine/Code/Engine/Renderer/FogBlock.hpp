#pragma once
#include "Engine\Math\Vector4.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
struct FogBlock
{
	Vector4 FOG_COLOR; 
	float	FOG_NEAR_PLANE;
	float	FOG_FAR_PLANE;
	float	FOG_NEAR_FACTOR;
	float	FOG_FAR_FACTOR;
};

