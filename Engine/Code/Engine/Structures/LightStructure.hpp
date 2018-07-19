#pragma once
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
struct LightStructure
{
	Vector4 color;    // alpha is intensity

	Vector3 position; 
	float spotFactor = 0.f; // For unified code path

	Vector3 direction; 
	float directionFactor = 0.f; 

	Vector3 attenuation = Vector3( 1.f, 0.f, 0.f ); 
	float dotInnerAngle; 

	Vector3 specAttenuation; 
	float dotOuterAngle;

	Vector3 padding;
	float isShadowCasting = 0.f;

	Matrix44 shadowVP;
};

