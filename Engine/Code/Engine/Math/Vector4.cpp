#pragma warning (disable:4201)

#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
Vector4::Vector4( const Vector3& copyFrom, float w ): x(copyFrom.x), y(copyFrom.y), z(copyFrom.z), w(w)
{

}