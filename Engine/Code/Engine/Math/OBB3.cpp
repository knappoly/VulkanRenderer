#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/AABB3.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes

//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
OBB3::OBB3(const Matrix44& spaceMat)
	: space(spaceMat)
	, inverseSpace(spaceMat.GetInverse())
{
	
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
OBB3::OBB3(const Vector3& center, const Vector3& forward /*= Vector3::FORWARD*/, const Vector3& right /*= Vector3::RIGHT*/, const Vector3& up /*= Vector3::UP */)
{
	space.SetFromBasis(right, up, forward, center);
	inverseSpace = space.GetInverse();
}

//-----------------------------------------------------------------------------------------------
// Sets the space and calculates the inverse matrix
//
void OBB3::SetSpace(const Matrix44& spaceMat)
{
	space = spaceMat;
	inverseSpace = space.GetInverse();
}

//-----------------------------------------------------------------------------------------------
// Sets the space from the given vectors
//
void OBB3::SetSpace(const Vector3& right, const Vector3& up, const Vector3& forward)
{
	SetSpace(Matrix44(right, up, forward, space.GetTranslation()));
	inverseSpace = space.GetInverse();
}

//-----------------------------------------------------------------------------------------------
// Sets the center point of the matrix
//
void OBB3::SetCenter(const Vector3& center)
{
	space.SetTranslation(center);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the point is inside the OBB
//
bool OBB3::DoesContaintPoint(const Vector3& point)
{
	Vector3 localPos = inverseSpace * Vector4(point, 1.f);
	return (Abs(localPos) < Vector3::ONE); // Inverse will invert the scale
}

//-----------------------------------------------------------------------------------------------
// Returns the corners of the OBB3
//
void OBB3::GetCorners(Vector3* out)
{
	AABB3 bounds = AABB3(Vector3::ZERO, Vector3::ONE);
	out[0] = space.TransformPosition3D(bounds.GetBackBottomLeft());
	out[1] = space.TransformPosition3D(bounds.GetBackBottomRight());
	out[2] = space.TransformPosition3D(bounds.GetBackTopRight());
	out[3] = space.TransformPosition3D(bounds.GetBackTopLeft());
	out[4] = space.TransformPosition3D(bounds.GetFrontBottomLeft());
	out[5] = space.TransformPosition3D(bounds.GetFrontBottomRight());
	out[6] = space.TransformPosition3D(bounds.GetFrontTopRight());
	out[7] = space.TransformPosition3D(bounds.GetFrontTopLeft());
}
