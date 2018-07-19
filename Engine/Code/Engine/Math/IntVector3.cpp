#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/Vector3.hpp"
#include <string>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor 
//
IntVector3::IntVector3(const Vector3& copyFrom)
{
	x = (int) copyFrom.x;
	y = (int) copyFrom.y;
	z = (int) copyFrom.z;
}

//-----------------------------------------------------------------------------------------------
// Operator +
//
const	IntVector3 IntVector3::operator+(const IntVector3& vecToAdd) const
{
	IntVector3 temp(x,y,z);
	temp.x += vecToAdd.x;
	temp.y += vecToAdd.y;
	temp.z += vecToAdd.z;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator - 
//
const	IntVector3 IntVector3::operator-(const IntVector3& vecToSubtract) const
{
	IntVector3 temp(x,y,z);
	temp.x -= vecToSubtract.x;
	temp.y -= vecToSubtract.y;
	temp.z -= vecToSubtract.z;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator =
//
void IntVector3::operator=(const IntVector3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

//-----------------------------------------------------------------------------------------------
// Operator ==
//
bool IntVector3::operator==(const IntVector3& compare) const
{
	if( x == compare.x && y == compare.y && z == compare.z)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Operator !=
//
bool IntVector3::operator!=(const IntVector3& compare) const
{
	if( x != compare.x && y != compare.y && z != compare.z)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Operator *
//
const	IntVector3 IntVector3::operator*(int uniformScale) const
{
	IntVector3 temp(x,y,z);
	temp.x *= uniformScale;
	temp.y *= uniformScale;
	temp.z *= uniformScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// String to intvector parser
//
void IntVector3::SetFromText(const char* text)
{
	std::string vecText = text;
	std::string newX, newY, newZ;

	StringTokenizer tokenizer(text, ",");
	tokenizer.Tokenize();

	Strings tokens = tokenizer.GetTokens();
	GUARANTEE_OR_DIE(tokenizer.GetTokenCount() == 3, "IntVector3 requires the format x,y,z")
	
	newX = tokens[0];
	newY = tokens[1];
	newZ = tokens[2];
	x = atoi(newX.c_str());
	y = atoi(newY.c_str());
	z = atoi(newZ.c_str());
}

//-----------------------------------------------------------------------------------------------
// Operator /
//
const	IntVector3 IntVector3::operator/(int inverseScale) const
{
	IntVector3 temp(x,y,z);
	temp.x /= inverseScale;
	temp.y /= inverseScale;
	temp.z /= inverseScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator /=
//
void IntVector3::operator/=(const int uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}

//-----------------------------------------------------------------------------------------------
// Operator *=
//
void IntVector3::operator*=(const int uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
// Operator +=
//
void IntVector3::operator+=(const IntVector3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

//-----------------------------------------------------------------------------------------------
// Operator -=
//
void IntVector3::operator-=(const IntVector3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

//-----------------------------------------------------------------------------------------------
// Operator *
//
const IntVector3 operator*(int uniformScale, const IntVector3& vecToScale)
{
	IntVector3 temp;
	temp.x = vecToScale.x * uniformScale;
	temp.y = vecToScale.y * uniformScale;
	temp.z = vecToScale.z * uniformScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Interpolates an intvector from the given start and end
//
const IntVector3 Interpolate(const IntVector3& start, const IntVector3& end, float fractionTowardEnd)
{
	IntVector3 temp;
	temp.x = Interpolate(start.x, end.x, fractionTowardEnd);
	temp.y = Interpolate(start.y, end.y, fractionTowardEnd);
	temp.z = Interpolate(start.z, end.z, fractionTowardEnd);
	return temp;
}
