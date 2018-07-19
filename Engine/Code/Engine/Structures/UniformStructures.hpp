#pragma once
#include "Engine/Structures/LightStructure.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
constexpr int MAX_LIGHTS = 8;

//-----------------------------------------------------------------------------------------------
struct LightBlock
{
	Vector4			ambient;
	LightStructure	lights[MAX_LIGHTS];
};

//-----------------------------------------------------------------------------------------------
struct SpecularBlock
{
	float	specFactor = 0.f;
	float	specPower = 1.f;
	Vector2 padding;
};

//-----------------------------------------------------------------------------------------------
struct CameraBlock
{
	Matrix44 view;
	Matrix44 projection;
// 	Vector3	 cameraPos;
// 	float	 padding;
};

