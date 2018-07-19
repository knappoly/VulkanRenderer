#include "Engine/Math/Plane.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
Plane::Plane(const Vector3& norm, const Vector3& pos)
{
	normal = norm;
	distance = DotProduct(normal, pos);
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	Vector3 e0 = b - a;
	Vector3 e1 = c - a;
	normal = CrossProduct(e1, e0);

	GUARANTEE_OR_DIE(normal.GetLength() != 0.f, "Bad points for plane constructor");
	
	normal.Normalize();
	distance = DotProduct(normal, a);
}

//-----------------------------------------------------------------------------------------------
// Returns the distance of the point wrt plane
//
float Plane::GetDistanceFromPlane(const Vector3& pos) const
{
	float dist = DotProduct(normal, pos);
	
	return dist - distance;
}

//-----------------------------------------------------------------------------------------------
// Returns true if point is in front of plane
//
bool Plane::IsPointInFront(const Vector3& pos) const
{
	return GetDistanceFromPlane(pos) > 0;
}

//-----------------------------------------------------------------------------------------------
// Flips the normal and distance 
//
void Plane::FlipNormal()
{
	normal = -1.f * normal;
	distance = -distance;
}
