#pragma warning (disable:4201)

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include <vector>
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Static constants
const Vector3 Vector3::ONE = Vector3(1.f, 1.f, 1.f);
const Vector3 Vector3::ZERO = Vector3(0.f, 0.f, 0.f);
const Vector3 Vector3::UP = Vector3(0.f, 1.f, 0.f);
const Vector3 Vector3::DOWN = Vector3(0.f, -1.f, 0.f);
const Vector3 Vector3::RIGHT = Vector3(1.f, 0.f, 0.f);
const Vector3 Vector3::LEFT = Vector3(-1.f, 0.f, 0.f);
const Vector3 Vector3::FORWARD = Vector3(0.f, 0.f, 1.f);
const Vector3 Vector3::BACK = Vector3(0.f, 0.f, -1.f);

typedef std::vector<std::string> Strings;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Vector3::Vector3( const Vector2& vec2 ): x(vec2.x), y(vec2.y), z(0.f)
{

}

//-----------------------------------------------------------------------------------------------
// Converts from IntVector3 to Vector3
//
Vector3::Vector3(const IntVector3& ivec3)
{
	x = (float) ivec3.x;
	y = (float) ivec3.y;
	z = (float) ivec3.z;
}

//-----------------------------------------------------------------------------------------------
// Converts from IntVec2 to Vector3
//
Vector3::Vector3(const IntVector2& ivec2)
{
	x = (float) ivec2.x;
	y = (float) ivec2.y;
	z = 0.f;
}

//-----------------------------------------------------------------------------------------------
// Operator +
//
const Vector3 Vector3::operator+(const Vector3& vecToAdd) const
{
	Vector3 temp; 
	temp.x = x + vecToAdd.x;
	temp.y = y + vecToAdd.y;
	temp.z = z + vecToAdd.z;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator -
//
const Vector3 Vector3::operator-(const Vector3& vecToSubtract) const
{
	Vector3 temp; 
	temp.x = x - vecToSubtract.x;
	temp.y = y - vecToSubtract.y;
	temp.z = z - vecToSubtract.z;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator *
//
const Vector3 Vector3::operator*(float uniformScale) const
{
	Vector3 temp; 
	temp.x = x * uniformScale;
	temp.y = y * uniformScale;
	temp.z = z * uniformScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator < (Component wise)
//
bool Vector3::operator<(const Vector3& compare) const
{
	return x < compare.x && y < compare.y && z < compare.z;
}

//-----------------------------------------------------------------------------------------------
// Operator > (Component wise)
//
bool Vector3::operator>(const Vector3& compare) const
{
	return x > compare.x && y > compare.y && z > compare.z;
}

//-----------------------------------------------------------------------------------------------
// Operator /
//
const Vector3 Vector3::operator/(float inverseScale) const
{
	Vector3 temp; 
	temp.x = x / inverseScale;
	temp.y = y / inverseScale;
	temp.z = z / inverseScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator +=
//
void Vector3::operator+=(const Vector3 & vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

//-----------------------------------------------------------------------------------------------
// Operator -=
//
void Vector3::operator-=(const Vector3 & vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

//-----------------------------------------------------------------------------------------------
// Operator *= 
//
void Vector3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
// Operator /=
//
void Vector3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}

//-----------------------------------------------------------------------------------------------
// Operator =
//
void Vector3::operator=(const Vector3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

//-----------------------------------------------------------------------------------------------
// Operator ==
//
bool Vector3::operator==(const Vector3& compare) const
{
	if(x == compare.x && y == compare.y && z == compare.z)
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------------------------
// Operator !=
// 
bool Vector3::operator!=(const Vector3 & compare) const
{
	if(x != compare.x || y != compare.y || z != compare.z)
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------------------------
// Returns length of the vector
//
float Vector3::GetLength() const
{
	return sqrtf((x*x) + (y*y) + (z*z));
}

//-----------------------------------------------------------------------------------------------
// Returns length of the vector squared
//
float Vector3::GetLengthSquared() const
{
	return (x*x) + (y*y) + (z*z);
}

//-----------------------------------------------------------------------------------------------
// Normalizes the vector and returns the old length
//
float Vector3::NormalizeAndGetLength()
{
	float temp = GetLength();
	x = x / temp;
	y = y / temp;
	z = z / temp;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Returns the normalized vector
//
Vector3 Vector3::GetNormalized() const
{
	Vector3 temp;
	float length = GetLength();

	if(length == 0.f)
	{
		return temp;
	}

	temp.x = x / length;
	temp.y = y / length;
	temp.z = z / length;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Normalizes the vector
//
Vector3 Vector3::Normalize()
{
	*this = GetNormalized();
	return *this;
}

//-----------------------------------------------------------------------------------------------
// Sets the vector3 from text
//
void Vector3::SetFromText(const char* text)
{
	//Tokenize the string for x
	StringTokenizer tokenizer(text, ",");
	Strings tokens;
	tokenizer.Tokenize();

	GUARANTEE_OR_DIE(tokenizer.GetTokenCount() == 3, "Error in Vector3 string format. It should be (x,y,z)");
	tokens = tokenizer.GetTokens();

	x = strtof(tokens[0].c_str(), nullptr);
	y = strtof(tokens[1].c_str(), nullptr);
	z = strtof(tokens[2].c_str(), nullptr);
}

//-----------------------------------------------------------------------------------------------
// Converts this vector3 to a comma separated string
//
std::string Vector3::ToString() const
{
	std::string text = Stringf("%0.3f,%0.3f,%0.3f", x,y,z);
	return text;
}

//-----------------------------------------------------------------------------------------------
// Swizzle for XX
//
Vector2 Vector3::xx() const
{
	return Vector2(x,x);
}

//-----------------------------------------------------------------------------------------------
// Swizzle for XY
//
Vector2 Vector3::xy() const
{
	return Vector2(x,y);
}

//-----------------------------------------------------------------------------------------------
// Swizzle for yy
// 
Vector2 Vector3::yy() const
{
	return Vector2(y,y);
}

//-----------------------------------------------------------------------------------------------
// Swizzle for YX
//
Vector2 Vector3::yx() const
{
	return Vector2(y,x);
}

//-----------------------------------------------------------------------------------------------
// Swizzle for ZX
//
Vector2 Vector3::zx() const
{
	return Vector2(z,x);
}

//-----------------------------------------------------------------------------------------------
// Swizzle for XZ
//
Vector2 Vector3::xz() const
{
	return Vector2(x,z);
}

//-----------------------------------------------------------------------------------------------
// Operator *
//
const Vector3 operator*(float uniformScale, const Vector3& vecToScale)
{
	Vector3 temp;
	temp.x = vecToScale.x * uniformScale;
	temp.y = vecToScale.y * uniformScale;
	temp.z = vecToScale.z * uniformScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Interpolates the vector
//
Vector3 Interpolate(const Vector3& start, const Vector3& end, float fractionTowardEnd)
{
	Vector3 temp;
	temp.x = Interpolate(start.x, end.x, fractionTowardEnd);
	temp.y = Interpolate(start.y, end.y, fractionTowardEnd);
	temp.z = Interpolate(start.z, end.z, fractionTowardEnd);
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Returns the spherical interpolated vec3
//
Vector3 Slerp(const Vector3& start, const Vector3& end, float fractionTowardEnd)
{
	float startLength = start.GetLength();
	float endLength = end.GetLength();

	float len = Interpolate( startLength, endLength, fractionTowardEnd );
	Vector3 unit = SlerpUnit( start / startLength, end / endLength, fractionTowardEnd ); 
	return len * unit;
}

//-----------------------------------------------------------------------------------------------
// Returns a unit slerp for the two vectors
//
Vector3 SlerpUnit(const Vector3& start, const Vector3& end, float fractionTowardEnd)
{
	float cosAngle = ClampFloat(DotProduct(start, end), -1.0f, 1.0f);
	float angle = AcosDegrees(cosAngle);
	if (IsNearlyEqual(angle, 0.f)) 
	{
		return Interpolate( start, end, fractionTowardEnd );
	} 
	else 
	{
		float pos_num = SinDegrees( fractionTowardEnd * angle );
		float neg_num = SinDegrees( (1.0f - fractionTowardEnd) * angle );
		float den = SinDegrees(angle);

		return (neg_num / den) * start + (pos_num / den) * end;
	}
}
