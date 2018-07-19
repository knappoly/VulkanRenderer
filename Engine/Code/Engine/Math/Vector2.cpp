#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <string>
#include "Vector3.hpp"

const Vector2 Vector2::ZERO = Vector2(0.f,0.f);
const Vector2 Vector2::ONE = Vector2(1.f,1.f);

//-----------------------------------------------------------------------------------------------
// Constructor
// 
Vector2::Vector2( const Vector2& copy )
	: x( copy.x )
	, y( copy.y )
{
	
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Vector2::Vector2( float initialX, float initialY )
	: x( initialX )
	, y( initialY )
{
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Vector2::Vector2(const IntVector2& copyFrom)
{
	x = static_cast<float>(copyFrom.x);
	y = static_cast<float>(copyFrom.y);
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Vector2::Vector2(const Vector3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

//-----------------------------------------------------------------------------------------------
// "+" Operator Overload
//
const Vector2 Vector2::operator + ( const Vector2& vecToAdd ) const
{
	float xAdd = x + vecToAdd.x;
	float yAdd = y + vecToAdd.y;
	return Vector2( xAdd, yAdd ); 
}


//-----------------------------------------------------------------------------------------------
// "-" Operator Overload
// 
const Vector2 Vector2::operator-( const Vector2& vecToSubtract ) const
{
	float xSub = x - vecToSubtract.x;
	float ySub = y - vecToSubtract.y;
	return Vector2( xSub, ySub ); 
}

//-----------------------------------------------------------------------------------------------
// "*" Operator Overload
//
const Vector2 Vector2::operator*( float uniformScale ) const
{
	return Vector2( x*uniformScale, y*uniformScale ); 
}


//-----------------------------------------------------------------------------------------------
// '/' Operator Overload
//
const Vector2 Vector2::operator/( float inverseScale ) const
{
	return Vector2( x/inverseScale, y/inverseScale ); 
}


//-----------------------------------------------------------------------------------------------
// '+=' Operator Overload
//
void Vector2::operator+=( const Vector2& vecToAdd )
{
	x += vecToAdd.x; 
	y += vecToAdd.y; 
}

//-----------------------------------------------------------------------------------------------
// '-=' Operator Overload
//
void Vector2::operator-=( const Vector2& vecToSubtract )
{
	x -= vecToSubtract.x; 
	y -= vecToSubtract.y; 
}


//-----------------------------------------------------------------------------------------------
// '*=' Operator Overload
//
void Vector2::operator*=( const float uniformScale )
{
	x *= uniformScale; 
	y *= uniformScale; 
}


//-----------------------------------------------------------------------------------------------
// '/=' Operator Overload
//
void Vector2::operator/=( const float uniformDivisor )
{
	x /= uniformDivisor; 
	y /= uniformDivisor; 
}


//-----------------------------------------------------------------------------------------------
// '=' Operator Overload
//
void Vector2::operator=( const Vector2& copyFrom )
{
	x = copyFrom.x; 
	y = copyFrom.y; 
}


//-----------------------------------------------------------------------------------------------
// '*' Operator Overload with Scalar multiplier
//
const Vector2 operator*( float uniformScale, const Vector2& vecToScale )
{
	Vector2 temp(vecToScale.x*uniformScale, vecToScale.y*uniformScale);
	return temp; 
}


//-----------------------------------------------------------------------------------------------
// '==' Operator Overload
//
bool Vector2::operator==( const Vector2& compare ) const
{
	if(x == compare.x && y==compare.y)
		return true;
	else
		return false; 
}


//-----------------------------------------------------------------------------------------------
// '!=' Operator Overload
//
bool Vector2::operator!=( const Vector2& compare ) const
{
	if(x!= compare.x || y!=compare.y)
		return true;
	else
		return false; 
}


//-----------------------------------------------------------------------------------------------
// Returns the length of the vector
//
float Vector2::GetLength() const
{
	return sqrtf((x*x) + (y*y));

}

//-----------------------------------------------------------------------------------------------
// Returns square of the length
//
float Vector2::GetLengthSquared() const
{
	return ((x*x) + (y*y));
}

//-----------------------------------------------------------------------------------------------
// Returns the length of the vector before the vector is normalized
//
float Vector2::NormalizeAndGetLength()
{
	float temp = GetLength();
	x = x / temp;
	y = y / temp;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Returns a normalized version of the vector 
//
Vector2 Vector2::GetNormalized() const
{
	Vector2 temp = Vector2(x,y) / GetLength();
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Returns the orientation of the degree 
//
float Vector2::GetOrientationDegrees() const
{
	return Atan2Degrees(y,x);
}

//-----------------------------------------------------------------------------------------------
// Returns a vector that is rotated "degrees" with respect to x-axis
// 
Vector2 Vector2::MakeDirectionAtDegrees(float degrees)
{
	return Vector2(CosDegrees(degrees),SinDegrees(degrees));

}

//-----------------------------------------------------------------------------------------------
// String to Vector2 parser
//
void Vector2::SetFromText(const char* text)
{
	std::string vecText = text;
	std::string newX, newY;
	size_t commaIndex = 0;

	//Tokenize the string for x
	commaIndex = vecText.find(",", commaIndex);
	GUARANTEE_OR_DIE(commaIndex > 0, "Vector2 requires the format x,y")
		newX = std::string(vecText, 0, commaIndex);

	//Tokenize the string for y
	newY = std::string(vecText, commaIndex+1);
	GUARANTEE_OR_DIE(!newY.empty(), "Vector2 requires the format x,y");

	x = static_cast<float>(atof(newX.c_str()));
	y = static_cast<float>(atof(newY.c_str()));
	//DebuggerPrintf("X: %f Y:%f", x,y);
}

//-----------------------------------------------------------------------------------------------
// Interpolates the value from the vector
//
const Vector2 Interpolate( const Vector2& start, const Vector2& end, float fractionTowardEnd )
{
	Vector2 temp;
	temp.x = Interpolate(start.x, end.x, fractionTowardEnd);
	temp.y = Interpolate(start.y, end.y, fractionTowardEnd);
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Returns the projection vector of the vector on the other vector
//
const Vector2 GetProjectedVector(const Vector2& vectorToProject, const Vector2& projectOnto)
{
	Vector2 projectOntoNormalized = projectOnto.GetNormalized();
	float dot = DotProduct(vectorToProject, projectOntoNormalized );
	
	Vector2 projectedVec = dot * projectOntoNormalized;
	return projectedVec;
}

//-----------------------------------------------------------------------------------------------
// Returns the vector’s representation/coordinates in (i,j) space (instead of its original x,y space)
//
const Vector2 GetTransformedIntoBasis(const Vector2& originalVector, const Vector2& newBasisI, const Vector2& newBasisJ)
{
	Vector2 transformedVector;
	transformedVector.x = DotProduct(originalVector, newBasisI.GetNormalized());
	transformedVector.y = DotProduct(originalVector, newBasisJ.GetNormalized());
	return transformedVector;
}

// Takes “vectorInBasis” in (i,j) space and returns the equivalent vector in [axis-aligned] (x,y) Cartesian space
const Vector2 GetTransformedOutOfBasis(const Vector2& vectorInBasis, const Vector2& oldBasisI, const Vector2& oldBasisJ)
{
	Vector2 transformedVectorX;
	Vector2 transformedVectorY;
	transformedVectorX =  vectorInBasis.x * oldBasisI; 
	transformedVectorY = vectorInBasis.y * oldBasisJ;
	return transformedVectorX + transformedVectorY;
}

//-----------------------------------------------------------------------------------------------
//   “vectorAlongI” is the vector portion in the “newBasisI” direction, and
//   “vectorAlongJ” is the vector portion in the “newBasisJ” direction.
//
void DecomposeVectorIntoBasis(const Vector2& originalVector, const Vector2& newBasisI, const Vector2& newBasisJ, Vector2& out_vectorAlongI, Vector2& out_vectorAlongJ)
{
	Vector2 transformedVector;
	transformedVector.x = DotProduct(originalVector, newBasisI);
	transformedVector.y = DotProduct(originalVector, newBasisJ);
	
	out_vectorAlongI = transformedVector.x * newBasisI * 1.f / newBasisI.GetLengthSquared();
	out_vectorAlongJ = transformedVector.y * newBasisJ * 1.f / newBasisJ.GetLengthSquared();
}

//-----------------------------------------------------------------------------------------------
// Returns the reflection vector
//
const Vector2 Reflect(const Vector2& inVector, const Vector2& normal)
{
	float inVectorProjection = DotProduct(inVector, normal);
	Vector2 inVectorAlongNormal =  inVectorProjection * normal * 1.f / normal.GetLengthSquared();
	Vector2 reflectedVector = inVector - 2.f * inVectorAlongNormal;
	return reflectedVector;
}

