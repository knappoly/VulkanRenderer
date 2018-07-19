#pragma once
#pragma warning (disable:4201)
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Vector2;
class IntVector3;
class IntVector2;

//-----------------------------------------------------------------------------------------------
class Vector3
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	~Vector3(){}
	Vector3():x(0.f),y(0.f),z(0.f){}
	explicit Vector3( float xyz ): x(xyz), y(xyz), z(xyz) {}
	explicit Vector3( float initialX, float initialY, float initialZ ): x(initialX), y(initialY), z(initialZ){ }
	Vector3( const Vector2& vec2 ); // Constructs a Vec3 from a Vec2
	explicit Vector3( float initialX, float initialY ): x(initialX), y(initialY), z(0.f){} // Default z is 0
	Vector3( const IntVector3& ivec3 ); // explicit and implicit
	Vector3( const IntVector2& ivec2 ); // explicit and implicit
	Vector3( const Vector3& copyFrom ): x(copyFrom.x), y(copyFrom.y), z(copyFrom.z){} // Copy constructor

	//-----------------------------------------------------------------------------------------------
	// Operators
			const	Vector3		operator+( const Vector3& vecToAdd ) const;					// vec3 + vec3
			const	Vector3		operator-( const Vector3& vecToSubtract ) const;			// vec3 - vec3
			const	Vector3		operator*( float uniformScale ) const;						// vec3 * float
			const	Vector3		operator/( float inverseScale ) const;						// vec3 / float
					void		operator+=( const Vector3& vecToAdd );						// vec3 += vec3
					void		operator-=( const Vector3& vecToSubtract );					// vec3 -= vec3
					void		operator*=( const float uniformScale );						// vec3 *= float
					void		operator/=( const float uniformDivisor );					// vec3 /= float
					void		operator=( const Vector3& copyFrom );						// vec3 = vec3
					bool		operator==( const Vector3& compare ) const;					// vec3 == vec3
					bool		operator!=( const Vector3& compare ) const;					// vec3 != vec3
	friend	const	Vector3		operator*( float uniformScale, const Vector3& vecToScale );	// float * vec3
					bool		operator<( const Vector3& compare ) const;					// vec3 < vec3
					bool		operator>( const Vector3& compare ) const;					// vec3 > vec3

	//-----------------------------------------------------------------------------------------------
	// Accessors and Mutators
	float		GetLength() const;
	float		GetLengthSquared() const ; // faster than GetLength() since it skips the sqrtf()
	float		NormalizeAndGetLength(); // set my new length to 1.0f; keep my direction
	Vector3		GetNormalized() const; // return a new vector, which is a normalized copy of me
	Vector3		Normalize();
	void		SetFromText( const char* text );
	std::string ToString() const;

	Vector3		xyz() const { return *this; }
	Vector2		xx() const;
	Vector2		xy() const; 
	Vector2		yy() const;
	Vector2		yx() const;
	Vector2		zx() const;
	Vector2		xz() const;


	//-----------------------------------------------------------------------------------------------
	// Members

	static const Vector3 ONE;
	static const Vector3 ZERO;
	static const Vector3 UP;
	static const Vector3 DOWN;
	static const Vector3 RIGHT;
	static const Vector3 LEFT;
	static const Vector3 FORWARD;
	static const Vector3 BACK;

	union 
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float r;
			float theta;
			float phi;
		};
		float data[3];
	};
	
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
Vector3 Interpolate( const Vector3& start, const Vector3& end, float fractionTowardEnd );
Vector3	Slerp( const Vector3& start, const Vector3& end, float fractionTowardEnd );
Vector3	SlerpUnit( const Vector3& start, const Vector3& end, float fractionTowardEnd );