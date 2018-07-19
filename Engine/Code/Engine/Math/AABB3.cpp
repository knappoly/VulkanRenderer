#include "Engine/Math/AABB3.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Math/MathUtils.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
AABB3::AABB3()
{
	mins = Vector3(-INFINITY);
	maxs = Vector3(INFINITY);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the AABB3 is uninitialized
//
bool AABB3::IsInvalid()
{
	return mins.x > maxs.x;
}

//-----------------------------------------------------------------------------------------------
// Returns the back bottom left corner
//
Vector3 AABB3::GetBackBottomLeft()
{
	return mins;
}

//-----------------------------------------------------------------------------------------------
// Returns the back bottom right corner
//
Vector3 AABB3::GetBackBottomRight()
{
	Vector3 br = mins + Vector3(GetSizes().x, 0.f, 0.f);
	return br;
}

//-----------------------------------------------------------------------------------------------
// Returns the back top right corner
//
Vector3 AABB3::GetBackTopRight()
{
	Vector3 sizes = GetSizes();
	Vector3 tr = mins + Vector3(sizes.x, sizes.y, 0.f);
	return tr;
}

//-----------------------------------------------------------------------------------------------
// Returns the back top left corner
//
Vector3 AABB3::GetBackTopLeft()
{
	Vector3 sizes = GetSizes();
	Vector3 tl = mins + Vector3(0.f, sizes.y, 0.f);
	return tl;
}

//-----------------------------------------------------------------------------------------------
// Returns the front bottom left
//
Vector3 AABB3::GetFrontBottomLeft()
{
	Vector3 sizes = GetSizes();
	Vector3 frontMins = maxs + Vector3(-sizes.x, -sizes.y, 0.f);
	return frontMins;
}

//-----------------------------------------------------------------------------------------------
// Returns the front bottom right
//
Vector3 AABB3::GetFrontBottomRight()
{
	Vector3 sizes = GetSizes();
	Vector3 frontBR = maxs + Vector3(0.f, -sizes.y, 0.f);
	return frontBR;
}

//-----------------------------------------------------------------------------------------------
// Returns the front top right
//
Vector3 AABB3::GetFrontTopRight()
{
	return maxs;
}

//-----------------------------------------------------------------------------------------------
// Returns the front top left
//
Vector3 AABB3::GetFrontTopLeft()
{
	Vector3 sizes = GetSizes();
	Vector3 frontTL = maxs + Vector3(-sizes.x, 0.f, 0.f);
	return frontTL;
}

//-----------------------------------------------------------------------------------------------
// Returns the center position
//
Vector3 AABB3::GetCenter()
{
	return (mins + maxs) * 0.5f;
}

//-----------------------------------------------------------------------------------------------
// Returns the half dimensions of the aabb3
//
Vector3 AABB3::GetHalfExtents()
{
	Vector3 halfSizes = maxs - mins;
	return Abs(halfSizes * 0.5f);
}

//-----------------------------------------------------------------------------------------------
// Returns the sizes on all sides
//
Vector3 AABB3::GetSizes()
{
	Vector3 sizes = maxs - mins;
	return Abs(sizes);
}

//-----------------------------------------------------------------------------------------------
// Sets the new center and calculates bounds from it
//
void AABB3::SetCenter(const Vector3& center)
{
	Vector3 halfExtents = GetHalfExtents();
	mins = center - halfExtents;
	maxs = center + halfExtents;
}

//-----------------------------------------------------------------------------------------------
// Translates the aabb3
//
void AABB3::Translate(const Vector3& translation)
{
	mins += translation;
	maxs += translation;
}

//-----------------------------------------------------------------------------------------------
// Modifies the aabb3 to contain the point
//
void AABB3::GrowToContain(const Vector3& pos)
{
	mins = Min(mins, pos);
	maxs = Max(maxs, pos);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the point is inside 
//
bool AABB3::IsPointInside(const Vector3& pos) const
{
	if(pos.x >= mins.x && pos.x <= maxs.x && pos.y >= mins.y && pos.z >= mins.z && pos.z <= maxs.z)
	{
		return true;
	}	
	
	return false;
}
