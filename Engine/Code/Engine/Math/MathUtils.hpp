#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Vector2;
class Vector3;
class Vector4;
class AABB2;
class Disc2;
class Segment3;
class Plane;
struct RaycastHit3D;
class Ray3;
class Matrix44;
class Disc3;

typedef unsigned int uint32_t;

//-----------------------------------------------------------------------------------------------
// Math Constants
const float M_PI = 3.14159f;
const float EPSILON = 0.0000001f;

//-----------------------------------------------------------------------------------------------
// General Utilities
int				Min( int a, int b );
uint32_t		Min( uint32_t a, uint32_t b );
float			Min( float a, float b );
Vector3			Min( const Vector3& a, const Vector3& b );
Vector2			Min( const Vector2& a, const Vector2& b );
Vector4			Min( const Vector4& a, const Vector4& b );
int				Max( int a, int b );
uint32_t		Max( uint32_t a, uint32_t b );
float			Max( float a, float b );
Vector3			Max( const Vector3& a, const Vector3& b );
Vector2			Max( const Vector2& a, const Vector2& b );
Vector4			Max( const Vector4& a, const Vector4& b );

float			Abs( float value );
Vector3			Abs( const Vector3& a );
Vector2			Abs( const Vector2& a );
Vector4			Abs( const Vector4& a );

float			Average( float a, float b );
Vector2			Average( const Vector2& a, const Vector2& b );
Vector3			Average( const Vector3& a, const Vector3& b );
Vector4			Average( const Vector4& a, const Vector4& b );

bool			IsNearlyEqual( float a, float b );

//-----------------------------------------------------------------------------------------------
// Angle and trigonometry functions
float			ConvertRadiansToDegrees(float radians);
float			ConvertDegreesToRadians(float degrees);
float			CosDegrees(float degrees);
float			SinDegrees(float degrees);
float			TanDegrees(float degrees);
float			Atan2Degrees(float y, float x);
float			AsinDegress(float value);
float			AcosDegrees(float value);
Vector2			PolarToCartesian(float radius, float degrees);
Vector2			CartesianToPolar(float x, float y);
Vector2			CartesianToPolar(const Vector2& cart);
Vector2			PolarToCartesian(const Vector2& polar);
Vector3			CartesianToSpherical(const Vector3& cart);
Vector3			SphericalToCartesian(const Vector3& spherical);
Vector3			SphericalToCartesian(float rad, float rotation, float azimuth);

//-----------------------------------------------------------------------------------------------
// Vector magnitude functions
float			GetDistance(  const Vector2& a, const  Vector2& b );
float			GetDistanceSquared( const Vector2& a, const Vector2& b );
float			GetDistanceSquared( const Vector3& a, const Vector3& b );

//-----------------------------------------------------------------------------------------------
// Geometry overlap check functions
bool			DoDiscsOverlap( const Disc2& a, const  Disc2& b);
bool			DoDiscsOverlap( const Vector2& aCenter, float aRadius, const Vector2& bCenter, float bRadius);
bool			DoSpheresOverlap( const Disc3& a, const Disc3& b );
bool			DoAABBsOverlap(const AABB2& a, const AABB2& b) ;
bool			DoesSegmentIntersectPlane( const Segment3& segment, const Plane& plane );
RaycastHit3D	RayCheckPlane( const Ray3& ray, const Plane& plane );

//-----------------------------------------------------------------------------------------------
// Random number generators
float			GetRandomFloatInRange(float minInclusive,float maxInclusive);
int				GetRandomIntInRange(int minInclusive,int maxInclusive);
float			GetRandomFloatZeroToOne();
int				GetRandomIntLessThan(int maxNotInclusive);
bool			CheckRandomChance(float chanceForSuccess);
Vector3			GetRandomPointOnSphere();

//-----------------------------------------------------------------------------------------------
// Range functions
int				RoundToNearestInt(float inValue);
float			RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd);
int				RangeMapInt(int inValue, int inStart, int inEnd, int outStart, int outEnd);
Vector2			RangeMap( const Vector2& inValue, const Vector2& inStart, const Vector2& inEnd, const Vector2& outStart, const Vector2& outEnd );
float			GetFractionInRange(float inValue, float rangeStart, float rangeEnd);

//-----------------------------------------------------------------------------------------------
// Clamp functions
float			ClampFloat(float inValue, float minInclusive, float maxInclusive);
float			ClampFloatZeroToOne(float inValue);
float			ClampFloatNegativeOneToOne(float inValue);
int				ClampInt(int inValue, int minInclusive, int maxInclusive);

//-----------------------------------------------------------------------------------------------
// Interpolation functions
float			Interpolate(float start, float end, float fractionTowardEnd);
int				Interpolate(int start, int end, float fractionTowardEnd);
unsigned char	Interpolate(unsigned char start, unsigned char end, float fractionTowardEnd);

//-----------------------------------------------------------------------------------------------
// Angle utilities, Dot Product and Cross Product
float			GetAngularDisplacement(float start, float end);
float			TurnToward(float currentDegrees, float goalDegrees, float maxTurnDegrees);
Matrix44		TurnToward(const Matrix44& target, const Matrix44& current, float maxTurnDegrees);
float			DotProduct(const Vector2& a, const Vector2& b);
float			DotProduct(const Vector3& a, const Vector3& b);
Vector3			CrossProduct(const Vector3& a, const Vector3& b);

//-----------------------------------------------------------------------------------------------
// Bit flag functions
bool			AreBitsSet( unsigned char bitFlags8, unsigned char flagsToCheck );
bool			AreBitsSet( unsigned int bitFlags32, unsigned int flagsToCheck );
void			SetBits( unsigned char& bitFlags8, unsigned char flagsToSet );
void			SetBits( unsigned int& bitFlags32, unsigned int flagsToSet );
void			ClearBits( unsigned char& bitFlags8, unsigned char flagToClear );
void			ClearBits( unsigned int& bitFlags32, unsigned int flagToClear );

//-----------------------------------------------------------------------------------------------
// Easing functions
float			SmoothStart2( float t ); // 2nd-degree smooth start (a.k.a. “quadratic ease in”)
float			SmoothStart3( float t ); // 3rd-degree smooth start (a.k.a. “cubic ease in”)
float			SmoothStart4( float t ); // 4th-degree smooth start (a.k.a. “quartic ease in”)
float			SmoothStop2( float t ); // 2nd-degree smooth start (a.k.a. “quadratic ease out”)
float			SmoothStop3( float t ); // 3rd-degree smooth start (a.k.a. “cubic ease out”)
float			SmoothStop4( float t ); // 4th-degree smooth start (a.k.a. “quartic ease out”)
float			SmoothStep3( float t ); // 3rd-degree smooth start/stop (a.k.a. “smoothstep”)

//-----------------------------------------------------------------------------------------------
// Parsing functions
//
void			SetFromText(const char* valueText, int& out_value); // Parses float from a string
void			SetFromText(const char* valueText, float& out_value); // Parses integer from a string
void			SetFromText(const char* valueText, char& out_value); // Parses char from string
void			SetFromText(const char* valueText, bool& out_value); // Parses boolean from string

//-----------------------------------------------------------------------------------------------
// Quadratic Equation Util
//
bool			Quadratic(Vector2* out, float a, float b, float c); // Solves for the variable and gets both values as reference			


