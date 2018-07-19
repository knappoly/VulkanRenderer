#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <string>
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/IntVector3.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
const IntVector2 IntVector2::ZERO = IntVector2(0,0);
const IntVector2 IntVector2::ONE = IntVector2(1,1);

//-----------------------------------------------------------------------------------------------
// Operator overload
//
bool IntVector2::operator==(const IntVector2 & otherVector) const
{
	if( x == otherVector.x && y == otherVector.y)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
IntVector2::IntVector2(const Vector2& copyFrom)
{
	x = static_cast<int>(copyFrom.x) ;
	y = static_cast<int>(copyFrom.y) ;

}

//-----------------------------------------------------------------------------------------------
// Constructor
//
IntVector2::IntVector2(const IntVector3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

//-----------------------------------------------------------------------------------------------
// Operator +
//
const IntVector2 IntVector2::operator+(const IntVector2 & vecToAdd) const
{
	IntVector2 temp(x,y);
	temp.x += vecToAdd.x;
	temp.y += vecToAdd.y;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator -
//
const IntVector2 IntVector2::operator-(const IntVector2 & vecToSubtract) const
{
	IntVector2 temp(x,y);
	temp.x -= vecToSubtract.x;
	temp.y -= vecToSubtract.y;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator *
//
const IntVector2 IntVector2::operator*(int uniformScale) const
{
	IntVector2 temp(x,y);
	temp.x *= uniformScale;
	temp.y *= uniformScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator /
//
const IntVector2 IntVector2::operator/(int inverseScale) const
{
	IntVector2 temp(x,y);
	temp.x /= inverseScale;
	temp.y /= inverseScale;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Operator += 
//
void IntVector2::operator+=(const IntVector2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}

//-----------------------------------------------------------------------------------------------
// Operator -=
//
void IntVector2::operator-=(const IntVector2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}

//-----------------------------------------------------------------------------------------------
// Operator *=
//
void IntVector2::operator*=(const int uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
// Operator /=
//
void IntVector2::operator/=(const int uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}

//-----------------------------------------------------------------------------------------------
// Operator =
//
void IntVector2::operator=(const IntVector2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

//-----------------------------------------------------------------------------------------------
// Operator !=
//
bool IntVector2::operator!=( const IntVector2& compare) const
{
	if( x != compare.x && y != compare.y )
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Operator *
//
const IntVector2 operator*(int uniformScale, const IntVector2& vecToScale)
{
	IntVector2 temp;
	temp.x = uniformScale * vecToScale.x;
	temp.y = uniformScale * vecToScale.y;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// String to intvector parser
//
void IntVector2::SetFromText(const char* text)
{
	std::string vecText = text;
	std::string newX, newY;
	size_t commaIndex = 0;

	//Tokenize the string for x
	commaIndex = vecText.find(",", commaIndex);
	GUARANTEE_OR_DIE(commaIndex > 0, "IntVector2 requires the format x,y")
	newX = std::string(vecText, 0, commaIndex);

	//Tokenize the string for y
	newY = std::string(vecText, commaIndex+1);
	GUARANTEE_OR_DIE(!newY.empty(), "IntVector2 requires the format x,y");

	x = atoi(newX.c_str());
	y = atoi(newY.c_str());
	//DebuggerPrintf("X: %d Y:%d", x,y);
}

//-----------------------------------------------------------------------------------------------
// Returns the vector length squared of the int vector
//
int IntVector2::GetLengthSquared() const
{
	return ((x*x) + (y*y));
}

//-----------------------------------------------------------------------------------------------
// Returns the vector length of the int vector
//
int IntVector2::GetLength() const
{
	return (int) sqrtf((float) GetLengthSquared());
}

//-----------------------------------------------------------------------------------------------
// Makes a comma separated string for the intvector
//
std::string IntVector2::ToString() const
{
	return Stringf("%d,%d", x, y);
}

//-----------------------------------------------------------------------------------------------
// Interpolates an int vector from the given start and end vectors 
//
const IntVector2 Interpolate( const IntVector2& start, const IntVector2& end, float fractionTowardEnd )
{
	IntVector2 temp;
	temp.x = Interpolate(start.x, end.x, fractionTowardEnd);
	temp.y = Interpolate(start.y, end.y, fractionTowardEnd);
	return temp;
}
