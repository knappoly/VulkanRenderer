#include "Engine/Math/Disc2.hpp"

//-----------------------------------------------------------------------------------------------
// Default constructor
// 
Disc2::Disc2()
{
	center = Vector2(0.f,0.f);
	radius = 0;		 
}
//-----------------------------------------------------------------------------------------------
// Copy constructor
//
Disc2::Disc2(const Disc2& copyFrom)
{
	center = copyFrom.center;
	radius = copyFrom.radius;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Disc2::Disc2(float initialX, float initialY, float initialRadius)
{
	center = Vector2(initialX,initialY);
	radius = initialRadius;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Disc2::Disc2(const Vector2& initialCenter, float initialRadius)
{
	center = initialCenter;
	radius = initialRadius;
}

//-----------------------------------------------------------------------------------------------
// Increase radius to include the given point
//
void Disc2::StretchToIncludePoint( const Vector2& point)
{
	StretchToIncludePoint(point.x,point.y);
}

//-----------------------------------------------------------------------------------------------
// Increase radius to include the given point
//
void Disc2::StretchToIncludePoint(float x, float y)
{
	Vector2 temp(x,y);
	float tempDistance = GetDistance(center,temp);
	if(tempDistance > radius)
	{
		radius = tempDistance;
	}

}

//-----------------------------------------------------------------------------------------------
// Add a padding radius
//
void Disc2::AddPaddingToRadius(float paddingRadius)
{
	radius += paddingRadius;
}

//-----------------------------------------------------------------------------------------------
// Translates the disc
//
void Disc2::Translate(const Vector2& translation)
{
	center += translation;
}

//-----------------------------------------------------------------------------------------------
// Translates the disc
//
void Disc2::Translate(float translationX, float translationY) 
{
	center += Vector2(translationX,translationY);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the point is inside
//
bool Disc2::IsPointInside(float x, float y) const
{
	Vector2 temp(x,y);
	return IsPointInside(temp);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the point is inside
//
bool Disc2::IsPointInside(const Vector2& point) const
{
	Vector2 temp = point;
	float distance = GetDistance(center,temp);
	if(distance < radius)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Operator -=
//
void Disc2::operator-=(const Vector2& antiTranslation)
{
	center -= antiTranslation;
}

//-----------------------------------------------------------------------------------------------
// Operator -
//
Disc2 Disc2::operator-(const Vector2& antiTranslation) const
{
	float xSub = center.x - antiTranslation.x;
	float ySub = center.x - antiTranslation.y;
	return Disc2(Vector2(xSub,ySub),radius);
}

//-----------------------------------------------------------------------------------------------
// Operator +=
//
void Disc2::operator+=(const Vector2& translation)
{
	center += translation;
}

//-----------------------------------------------------------------------------------------------
// Operator '+'
//
Disc2 Disc2::operator+(const Vector2& translation) const
{
	float xAdd = center.x + translation.x;
	float yAdd = center.x + translation.y;
	return Disc2(Vector2(xAdd,yAdd),radius);;
}

//-----------------------------------------------------------------------------------------------
// Interpolates the disc positions
//
const Disc2 Interpolate( const Disc2& start, const Disc2& end, float fractionTowardEnd )
{
	Disc2 temp;
	temp.center = Interpolate(start.center, end.center, fractionTowardEnd);
	temp.radius = Interpolate(start.radius, end.radius, fractionTowardEnd);
	return temp;
}
