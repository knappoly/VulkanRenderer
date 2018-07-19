#include "Engine/Math/Disc3.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Math/MathUtils.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
Disc3::Disc3()
{

}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Disc3::Disc3(const Vector3& initCenter, float initRadius)
{
	center = initCenter;
	radius = initRadius;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Disc3::Disc3(float x, float y, float z, float initRadius)
{
	center = Vector3(x,y,z);
	radius = initRadius;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the point is inside the sphere
//
bool Disc3::IsPointInside(float x, float y, float z) const
{
	return IsPointInside(Vector3(x,y,z));
}

//-----------------------------------------------------------------------------------------------
// Returns true if the point is inside the sphere
//
bool Disc3::IsPointInside(const Vector3& point) const
{
	Vector3 displacement = point - center;
	float radiusSq = radius * radius;
	float distanceToPointSq = displacement.GetLengthSquared();

	return distanceToPointSq < radiusSq;

}

//-----------------------------------------------------------------------------------------------
// Stretches the disc3 to include the point
//
void Disc3::StretchToIncludePoint(float x, float y, float z)
{
	StretchToIncludePoint(Vector3(x,y,z));
}

//-----------------------------------------------------------------------------------------------
// Stretches the disc3 to include the point 
//
void Disc3::StretchToIncludePoint(const Vector3& point)
{
	Vector3 displacement = point - center;
	float radiusSq = radius * radius;
	float distanceToPointSq = displacement.GetLengthSquared();

	if(distanceToPointSq > radiusSq)
	{
		radius = distanceToPointSq;
	}
}

//-----------------------------------------------------------------------------------------------
// Adds padding to the radius
//
void Disc3::AddPaddingToRadius(float paddingRadius)
{
	radius += paddingRadius;
}

//-----------------------------------------------------------------------------------------------
// Translates the disc3
//
void Disc3::Translate(const Vector3& translation)
{
	center += translation;
}

//-----------------------------------------------------------------------------------------------
// Translates the disc3
//
void Disc3::Translate(float translationX, float translationY, float translationZ)
{
	Translate(Vector3(translationX, translationY, translationZ));
}

//-----------------------------------------------------------------------------------------------
// Interpolates the disc3
//
const Disc3 Interpolate(const Disc3& start, const Disc3& end, float fractionTowardEnd)
{
	Disc3 lerpDisc;
	lerpDisc.center = Interpolate(start.center, end.center, fractionTowardEnd);
	lerpDisc.radius = Interpolate(start.radius, end.radius, fractionTowardEnd);

	return lerpDisc;
}
