#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Disc2.hpp"
#include "Engine/Math/AABB2.hpp"
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <string>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Segment3.hpp"
#include "Engine/Math/Plane.hpp"
#include "Engine/Math/RaycastHit3D.hpp"
#include "Engine/Math/Ray3.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/Disc3.hpp"

//-----------------------------------------------------------------------------------------------
// Returns the min between the values
//
float Min(float a, float b)
{
	float min = (a>b) ? b : a;
	return min;
}

//-----------------------------------------------------------------------------------------------
// Returns the min between the vec3 (component wise)
//
Vector3 Min(const Vector3& a, const Vector3& b)
{
	return Vector3( Min(a.x,b.x), Min(a.y,b.y), Min(a.z,b.z) );
}

//-----------------------------------------------------------------------------------------------
// Returns the min between the vec2 (component wise)
//
Vector2 Min(const Vector2& a, const Vector2& b)
{
	return Vector2( Min(a.x,b.x), Min(a.y,b.y) );
}

//-----------------------------------------------------------------------------------------------
// Returns the min between the vec4(component wise)
//
Vector4 Min(const Vector4& a, const Vector4& b)
{
	return Vector4( Min(a.x,b.x), Min(a.y,b.y), Min(a.z,b.z), Min(a.w,b.w) );
}

//-----------------------------------------------------------------------------------------------
// Returns the min between the two integers
//
int Min(int a, int b)
{
	int min = (a>b) ? b : a;
	return min;
}

//-----------------------------------------------------------------------------------------------
// Returns the min between the two unsigned ints
//
uint32_t Min(uint32_t a, uint32_t b)
{
	uint32_t min = (a>b) ? b : a;
	return min;
}

//-----------------------------------------------------------------------------------------------
// Returns max of the two floats
//
float Max(float a, float b)
{
	float max = (a>b) ? a : b;
	return max;
}

//-----------------------------------------------------------------------------------------------
// Returns max of the two vec3(component wise)
//
Vector3 Max(const Vector3& a, const Vector3& b)
{
	return Vector3( Max(a.x,b.x), Max(a.y,b.y), Max(a.z,b.z) );
}

//-----------------------------------------------------------------------------------------------
// Returns the max of the two vec2(component wise)
//
Vector2 Max(const Vector2& a, const Vector2& b)
{
	return Vector2( Max(a.x,b.x), Max(a.y,b.y) );
}

//-----------------------------------------------------------------------------------------------
// Returns the max of the two vec4(component wise)
//
Vector4 Max(const Vector4& a, const Vector4& b)
{
	return Vector4( Max(a.x,b.x), Max(a.y,b.y), Max(a.z,b.z), Max(a.w,b.w) );
}

//-----------------------------------------------------------------------------------------------
// Returns the max of the two integers
//
int Max(int a, int b)
{
	int max = (a>b) ? a : b;
	return max;
}

//-----------------------------------------------------------------------------------------------
// Returns the max of the two unsigned ints
//
uint32_t Max(uint32_t a, uint32_t b)
{
	uint32_t max = (a>b) ? a : b;
	return max;
}

//-----------------------------------------------------------------------------------------------
// Returns the absolute value
//
float Abs(float value)
{
	return fabsf(value);
}

//-----------------------------------------------------------------------------------------------
// Returns the absolute value of the vec3 (Component wise)
//
Vector3 Abs(const Vector3& a)
{
	return Vector3(Abs(a.x), Abs(a.y), Abs(a.z));
}

//-----------------------------------------------------------------------------------------------
// Returns the absolute value of the vec2 (Component wise)
//
Vector2 Abs(const Vector2& a)
{
	return Vector2(Abs(a.x), Abs(a.y));
}

//-----------------------------------------------------------------------------------------------
// Returns the absolute value of the vec4 (Component wise)
//
Vector4 Abs(const Vector4& a)
{
	return Vector4(Abs(a.x), Abs(a.y), Abs(a.z), Abs(a.w));
}

//-----------------------------------------------------------------------------------------------
// Average of two floats
//
float Average(float a, float b)
{
	return (a + b) * 0.5f;
}

//-----------------------------------------------------------------------------------------------
// Average of vec2 (component wise)
//
Vector2 Average(const Vector2& a, const Vector2& b)
{
	return Vector2(Average(a.x, b.x), Average(a.y,b.y));
}

//-----------------------------------------------------------------------------------------------
// Average of vec3 (component wise)
//
Vector3 Average(const Vector3& a, const Vector3& b)
{
	return Vector3(Average(a.x, b.x), Average(a.y,b.y), Average(a.z,b.z));
}

//-----------------------------------------------------------------------------------------------
// Average of vec4(component wise)
//
Vector4 Average(const Vector4& a, const Vector4& b)
{
	return Vector4(Average(a.x, b.x), Average(a.y,b.y), Average(a.z,b.z), Average(a.w,b.w));
}

//-----------------------------------------------------------------------------------------------
// Returns true if the two are almost equal with an a very very minor error tolerance
//
bool IsNearlyEqual(float a, float b)
{
	float diff = Abs(b - a);
	if ( diff <= EPSILON )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
// Converts the given radians to degree
//
float ConvertRadiansToDegrees(float radians)
{
	float temp = 180.f / M_PI;
	return radians * temp;

}

//-----------------------------------------------------------------------------------------------
// Converts the given degree to radians
//
float ConvertDegreesToRadians(float degrees)
{

	float temp = M_PI/180.f;
	return degrees * temp;
}

//-----------------------------------------------------------------------------------------------
// Return cosine of the given degree
//
float CosDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return cosf(radians);

}

//-----------------------------------------------------------------------------------------------
// Calculates the arc-tangent 
//
float Atan2Degrees(float y, float x)
{
	return ConvertRadiansToDegrees(atan2f(y,x));
}

//-----------------------------------------------------------------------------------------------
// Calculates the arc-sin of the value 
//
float AsinDegress(float value)
{
	return ConvertRadiansToDegrees(asinf(value));
}

//-----------------------------------------------------------------------------------------------
// Calculates the arc-cosine of the value
//
float AcosDegrees(float value)
{
	return ConvertRadiansToDegrees(acosf(value));
}

//-----------------------------------------------------------------------------------------------
// Converts Polar coordinates to cartesian 
//
Vector2 PolarToCartesian(float radius, float degrees)
{
	Vector2 cart;
	cart.x = radius * CosDegrees(degrees);
	cart.y = radius * SinDegrees(degrees);
	return cart;
}

//-----------------------------------------------------------------------------------------------
// Converts Polar coordinates to cartesian 
//
Vector2 PolarToCartesian(const Vector2& polar)
{
	return PolarToCartesian(polar.x, polar.y);
}

//-----------------------------------------------------------------------------------------------
// Converts Cartesian to Spherical Coordinates 
//
Vector3 CartesianToSpherical(const Vector3& cart)
{
	Vector3 spherical;
	spherical.r = sqrtf((cart.x * cart.x) + (cart.y * cart.y) + (cart.z * cart.z));
	spherical.theta = Atan2Degrees(cart.z, cart.x);
	spherical.phi = Atan2Degrees(cart.y, cart.z);
	return spherical;
}

//-----------------------------------------------------------------------------------------------
// Converts Spherical to Cartesian
//
Vector3 SphericalToCartesian(const Vector3& spherical)
{
	Vector3 cart;
	cart.x = spherical.r * CosDegrees(spherical.theta) * CosDegrees(spherical.phi);
	cart.y = spherical.r * SinDegrees(spherical.phi);
	cart.z = spherical.r * SinDegrees(spherical.theta) * CosDegrees(spherical.phi);
	return cart;
}

//-----------------------------------------------------------------------------------------------
// Converts Spherical to Cartesian
//
Vector3 SphericalToCartesian(float rad, float rotation, float azimuth)
{
	return SphericalToCartesian(Vector3(rad, rotation, azimuth));
}

//-----------------------------------------------------------------------------------------------
// Converts Cartesian coords to polar
//
Vector2 CartesianToPolar(float x, float y)
{
	Vector2 polar;
	polar.y = Atan2Degrees(y,x);
	polar.x = sqrtf((x*x + y*y));
	return polar;
}

//-----------------------------------------------------------------------------------------------
// Converts Cartesian coords to polar
//
Vector2 CartesianToPolar(const Vector2& cart)
{
	return CartesianToPolar(cart.x, cart.y);
}

//-----------------------------------------------------------------------------------------------
// Return sine of the given degree
//
float SinDegrees(float degrees)
{

	float radians = ConvertDegreesToRadians(degrees);
	return sinf(radians);

}

//-----------------------------------------------------------------------------------------------
// Returns the tan of the angle
//
float TanDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return tanf(radians);
}

//-----------------------------------------------------------------------------------------------
// Returns the distance between the vectors
//
float GetDistance( const Vector2& a, const Vector2& b) 
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrtf((dx*dx) + (dy*dy));

}


//-----------------------------------------------------------------------------------------------
// Returns the square of the distance between the vectors
//
float GetDistanceSquared( const Vector2& a, const Vector2& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return ((dx*dx) + (dy*dy));
}

//-----------------------------------------------------------------------------------------------
// Returns the square of the distance between the vectors
//
float GetDistanceSquared(const Vector3& a, const Vector3& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return ((dx*dx) + (dy*dy) + (dz*dz));
}

//-----------------------------------------------------------------------------------------------
// Checks if the discs overlap 
//
bool DoDiscsOverlap( const Disc2& a, const Disc2& b)
{
	float distance = GetDistance(a.center,b.center);
	if( distance <= (a.radius + b.radius) )
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Checks if discs overlap
//
bool DoDiscsOverlap( const Vector2& aCenter, float aRadius, const Vector2& bCenter, float bRadius)
{	
	Disc2 temp1(aCenter,aRadius);
	Disc2 temp2(bCenter,bRadius);
	return DoDiscsOverlap( temp1,temp2);
}

//-----------------------------------------------------------------------------------------------
// Checks if spheres overlap
//
bool DoSpheresOverlap(const Disc3& a, const Disc3& b)
{
	float distanceSq = GetDistanceSquared(a.center,b.center);
	float sumOfRadiiSq = (a.radius + b.radius);
	sumOfRadiiSq *= sumOfRadiiSq;

	if( distanceSq <= sumOfRadiiSq )
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Checks if the aabbs overlap
//
bool DoAABBsOverlap(const AABB2& a, const AABB2& b)
{

	Vector2 aTopLeft = Vector2(a.mins.x,a.maxs.y);
	Vector2 aBotRight = Vector2(a.maxs.x, a.mins.y);

	if(b.IsPointInside(a.maxs) || b.IsPointInside(a.mins) || b.IsPointInside(aTopLeft) || b.IsPointInside(aBotRight))
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the line segment intersects with the plane
//
bool DoesSegmentIntersectPlane(const Segment3& segment, const Plane& plane)
{
	float distStart = plane.GetDistanceFromPlane(segment.start);
	float distEnd = plane.GetDistanceFromPlane(segment.end);

	return (distStart * distEnd) < 0.f;
}

//-----------------------------------------------------------------------------------------------
// Checks if ray intersects plane
//
RaycastHit3D RayCheckPlane(const Ray3& ray, const Plane& plane)
{
	Vector3 p0 = ray.Evaluate(0.f);
	Vector3 p1 = ray.Evaluate(1.f);

	if( plane.GetDistanceFromPlane(p0) == plane.GetDistanceFromPlane(p1) )
	{
		return RaycastHit3D(); // Did not hit
	}

	float dir = DotProduct(plane.normal, ray.dir);
	float distance = plane.GetDistanceFromPlane(ray.start);

	if( (dir * distance) > 0.f )
	{
		return RaycastHit3D();
	}
	
	float velocity = DotProduct(ray.dir, plane.normal);
	if(velocity == 0.f)
	{
		return RaycastHit3D();
	}

	float time = -distance * 1.f / velocity;

	RaycastHit3D hitResult;
	hitResult.hit = true;
	hitResult.position = ray.Evaluate(time);
	hitResult.normal = plane.normal;
	
	return hitResult;
}

//-----------------------------------------------------------------------------------------------
// Return a random float in the given range
//
float GetRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float range = (maxInclusive - minInclusive) + 1;
	return (float) minInclusive + (float) GetRandomIntLessThan((int)range);
}

//-----------------------------------------------------------------------------------------------
// Return a random integer in the given range
//
int GetRandomIntInRange(int minInclusive, int maxInclusive)
{
	int range = (maxInclusive - minInclusive) + 1;
	return minInclusive + GetRandomIntLessThan(range);
}

//-----------------------------------------------------------------------------------------------
// Return a random float from zero to one
//
float GetRandomFloatZeroToOne()
{
	return (float)rand()/(float)RAND_MAX;
}

//-----------------------------------------------------------------------------------------------
// Return a random integer less than the given number
//
int GetRandomIntLessThan(int maxNotInclusive)
{
	return rand()%maxNotInclusive;
}

//-----------------------------------------------------------------------------------------------
// Returns true depending on the probability of success
// 
bool CheckRandomChance(float chanceForSuccess)
{
	
	float chance = GetRandomFloatZeroToOne();
	if(chance < chanceForSuccess)
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------------------------
// Gets a random point on a sphere
//
Vector3 GetRandomPointOnSphere()
{
	Vector3 spherical;
	spherical.r = 1.f;
	spherical.theta = GetRandomFloatInRange(0.f, 360.f);
	spherical.phi = GetRandomFloatInRange(-90.f, 90.f);

	return SphericalToCartesian(spherical);
}

//-----------------------------------------------------------------------------------------------
// Clamps integer to a range of int
//
int ClampInt(int inValue, int minInclusive, int maxInclusive)
{
	if(inValue >= maxInclusive)
	{
		return maxInclusive;
	}

	else if(inValue <= minInclusive)
	{
		return minInclusive;
	}

	else
	{
		return inValue;
	}
}

//-----------------------------------------------------------------------------------------------
// Interpolates the float value from the given range and fraction
//
float Interpolate(float start, float end, float fractionTowardEnd)
{
	float range = end - start;
	float fractionValue = fractionTowardEnd * range;
	
	return fractionValue + start;
}

//-----------------------------------------------------------------------------------------------
// Interpolates the integer value from the given range and fraction
//
int Interpolate(int start, int end, float fractionTowardEnd)
{
	if(fractionTowardEnd == 1.f)
		return end;
	else if(fractionTowardEnd == 0.f)
		return start;
	else
	{
		int numIntervals = (end - start) + 1;
		int fractionValue = start + static_cast<int>(fractionTowardEnd * (float) numIntervals);
		return fractionValue;
	}
	
}

//-----------------------------------------------------------------------------------------------
// Interpolates the byte value from the given range and fraction
//
unsigned char Interpolate(unsigned char start, unsigned char end, float fractionTowardEnd)
{
	//unsigned char range = end - start;
	unsigned char fractionValue = (unsigned char) Interpolate((int) start, (int)end, fractionTowardEnd);
	if(fractionValue > 255)
		return 255;
	else
		return fractionValue;
}

//-----------------------------------------------------------------------------------------------
// Returns the angular displacement between the angles
//
float GetAngularDisplacement(float start, float end)
{
	float angDisplacement = end - start;
	
	while(angDisplacement > 180.f)
	{
		angDisplacement -= 360.f;
	}
	while(angDisplacement < -180.f)
	{
		angDisplacement += 360.f;
	}
	return angDisplacement;
}

//-----------------------------------------------------------------------------------------------
// Returns the scalar product (dot product) of the vectors
//
float DotProduct(const Vector2& a, const Vector2& b)
{
	return ((a.x*b.x) + (a.y*b.y));
}

//-----------------------------------------------------------------------------------------------
// Returns the scalar product (dot product) of the vectors
//
float DotProduct(const Vector3& a, const Vector3& b)
{
	return ((a.x*b.x) + (a.y*b.y) + (a.z*b.z));
}

//-----------------------------------------------------------------------------------------------
// Returns the vector product (cross product) of the vectors
//
Vector3 CrossProduct(const Vector3& a, const Vector3& b)
{
	Vector3 result;
	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);

	return result;
}

//-----------------------------------------------------------------------------------------------
// Returns orientation after turning toward goal upto maxTurnDegrees per turn
//
float TurnToward(float currentDegrees, float goalDegrees, float maxTurnDegrees)
{
	float angularDisplacement = GetAngularDisplacement(currentDegrees,goalDegrees);
	if(angularDisplacement < 0)
	{
		if(angularDisplacement >= maxTurnDegrees)
			return goalDegrees;
		else if(angularDisplacement > 180.f || angularDisplacement < -180.f)
			return currentDegrees + maxTurnDegrees;
		else
			return (currentDegrees - maxTurnDegrees);
	}
	else
	{
		if(angularDisplacement < maxTurnDegrees)
			return goalDegrees;
		else
			return (maxTurnDegrees + currentDegrees);
	}
	

}

//-----------------------------------------------------------------------------------------------
// Turns towards the new matrix given upto maxTurnDegrees
//
Matrix44 TurnToward(const Matrix44& target, const Matrix44& current, float maxTurnDegrees)
{
	Matrix44 currentInverse = current.GetInverse();
	Matrix44 rotation = target * currentInverse;
	Matrix44 mat;

	float trace = rotation.GetTrace3();
	float cosTheta = (trace - 1.f) * 0.5f;
	cosTheta = ClampFloatNegativeOneToOne(cosTheta);
	float theta = AcosDegrees(cosTheta);
	
	float fractionTowardEnd = Min(maxTurnDegrees / theta, 1.f);
	mat = Interpolate(current, target, fractionTowardEnd);

	mat.SetTranslation(current.GetTranslation());
	return mat;
}

//-----------------------------------------------------------------------------------------------
// Returns the nearest int
//
int RoundToNearestInt(float inValue)
{
	float rounded = floorf(inValue);
	rounded = inValue - rounded;
	if(inValue < 0.f)
	{
		if(rounded < 0.5f)
		{
			return static_cast<int>(floorf(inValue));
		}
		else
		{
			return static_cast<int>(ceilf(inValue));
		}
		
	}
	else
	{
		return static_cast<int>(roundf(inValue));
	}
	
}

//-----------------------------------------------------------------------------------------------
// Coverts the range of values from one system to another
//
float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float inRange = inEnd - inStart;
	
	float outRange = outEnd - outStart;
	float inRelativeToStart = inValue - inStart;

	if(inRange == 0)
		return ((outStart+outEnd) * 0.5f);
	float fractionIntoRange = inRelativeToStart * 1.f/inRange;
	float outRelativeToStart = fractionIntoRange * outRange;

	return (outRelativeToStart + outStart);
}

//-----------------------------------------------------------------------------------------------
// Coverts the range of int values from one system to another
//
int RangeMapInt(int inValue, int inStart, int inEnd, int outStart, int outEnd)
{
	return static_cast<int>(RangeMapFloat((float) inValue, (float) inStart, (float) inEnd, (float) outStart, (float) outEnd));
}

//-----------------------------------------------------------------------------------------------
// Coverts the range of vec2 from one system to another
//
Vector2 RangeMap(const Vector2& inValue, const Vector2& inStart, const Vector2& inEnd, const Vector2& outStart, const Vector2& outEnd)
{
	Vector2 result;
	result.x = RangeMapFloat(inValue.x, inStart.x, inEnd.x, outStart.x, outEnd.x);
	result.y = RangeMapFloat(inValue.y, inStart.y, inEnd.y, outStart.y, outEnd.y);

	return result;
}

float GetFractionInRange(float inValue, float rangeStart, float rangeEnd)
{
	float range = rangeEnd - rangeStart;
	float valueRelativeToZero = fabs(inValue - rangeStart);
	return valueRelativeToZero * 1.f/fabsf(range);
}

//-----------------------------------------------------------------------------------------------
// Clamps the value [minInclusive,maxInclusive]
//
float ClampFloat(float inValue, float minInclusive, float maxInclusive)
{
	if(inValue > maxInclusive)
		return maxInclusive;

	if(inValue < minInclusive)
		return minInclusive;
	
	else
		return inValue;
}

//-----------------------------------------------------------------------------------------------
// Clamps the value to [0,1]
//
float ClampFloatZeroToOne(float inValue)
{
	if(inValue > 1.f)
		return 1.f;

	if(inValue < 0.f)
		return 0.f;
	
	else
		return inValue;
}

//-----------------------------------------------------------------------------------------------
//  Clamps the value to [-1,1]
//
float ClampFloatNegativeOneToOne(float inValue)
{
	if(inValue < -1.f)
		return -1.f;

	else if(inValue > 1.f)
		return 1.f;

	else
		return inValue;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the bits are ON
//
bool AreBitsSet( unsigned char bitFlags8, unsigned char flagsToCheck )
{
	unsigned char checkStatus = bitFlags8 & flagsToCheck;
	if(checkStatus == flagsToCheck)
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the bits are ON
//
bool AreBitsSet( unsigned int bitFlags32, unsigned int flagsToCheck )
{
	unsigned int checkStatus = bitFlags32 & flagsToCheck;
	if(checkStatus == flagsToCheck)
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------------------------
// Sets the bits to ON
//
void SetBits( unsigned char& bitFlags8, unsigned char flagsToSet )
{
	bitFlags8 = bitFlags8 | flagsToSet;
}

//-----------------------------------------------------------------------------------------------
// Sets the bits to ON
//
void SetBits( unsigned int& bitFlags32, unsigned int flagsToSet )
{
	bitFlags32 = bitFlags32 | flagsToSet;
}

//-----------------------------------------------------------------------------------------------
// Clears the bits
//
void ClearBits( unsigned char& bitFlags8, unsigned char flagToClear )
{
	unsigned char NAND = ~flagToClear;
	bitFlags8 = bitFlags8 & NAND;
}

//-----------------------------------------------------------------------------------------------
// Clears the bits
//
void ClearBits( unsigned int& bitFlags32, unsigned int flagToClear )
{
	unsigned int NAND = ~flagToClear;
	bitFlags32 = bitFlags32 & NAND;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
//
float SmoothStart2( float t )
{
	return t*t;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
//
float SmoothStart3( float t )
{
	return t*t*t;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
//
float SmoothStart4( float t )
{
	return t*t*t*t;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
// 
float SmoothStop2( float t )
{
	float smoothStop = 1 - ((1-t) * (1-t));
	return smoothStop;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
// 
float SmoothStop3( float t )
{
	float smoothStop = 1 - ((1-t) * (1-t) * (1-t));
	return smoothStop;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
// 
float SmoothStop4( float t )
{
	float smoothStop = 1 - ((1-t) * (1-t) * (1-t) * (1-t));
	return smoothStop;
}

//-----------------------------------------------------------------------------------------------
// Returns the value for the easing function
// 
float SmoothStep3( float t )
{
	float smoothStep = (1-t) * SmoothStart2(t) + t * SmoothStop2(t);
	return smoothStep ;
}

//-----------------------------------------------------------------------------------------------
// String to integer parser
//
void SetFromText(const char* valueText, int& out_value)
{
	int value = atoi(valueText);
	out_value = value;
}

//-----------------------------------------------------------------------------------------------
// String to float parser
//
void SetFromText(const char* valueText, float& out_value)
{
	float value = static_cast<float>(atof(valueText));
	out_value = value;
}

//-----------------------------------------------------------------------------------------------
// String to char parser
//
void SetFromText(const char* valueText, char& out_value)
{
	char value = valueText[0];
	out_value = value;
	//DebuggerPrintf("value: %c", value);
}

//-----------------------------------------------------------------------------------------------
// String to boolean parser
//
void SetFromText(const char* valueText, bool& out_value)
{
	bool value;
	std::string text = valueText;
	if(text == "true")
		value = true;
	else
		value = false;
	out_value = value;
}

//-----------------------------------------------------------------------------------------------
// Solves roots using quadratic formula
//
bool Quadratic(Vector2* out, float a, float b, float c)
{
	float bSquared = b * b;
	float deter = bSquared - (4.f * a * c);

	if(deter < 0.f)
	{
		return false;
	}

	deter = sqrtf(deter);
	out->x = (-b + deter) / (2.f * a);

	if(deter == 0.f)
	{
		out->y = out->x;
	}
	else
	{
		out->y = (-b - deter) / (2.f * a);
	}
	
	return true;
}
