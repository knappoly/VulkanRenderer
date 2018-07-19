#pragma once
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

class IntVector2;
class Vector3;

//-----------------------------------------------------------------------------------------------
class Vector2
{
public:
	//-----------------------------------------------------------------------------------------------
	// Construction/Destruction
	~Vector2() {}											// destructor: do nothing (for speed)
	Vector2() {}											// default constructor: do nothing (for speed)
	Vector2( float xy ): x(xy), y(xy) {}
	Vector2( const Vector2& copyFrom );						// copy constructor (from another vec2)
	Vector2( const Vector3& copyFrom );						// Implicit or Explicit constructor
	explicit Vector2( const IntVector2& copyFrom );			// explicit constructor 
	explicit Vector2( float initialX, float initialY );		// explicit constructor (from x, y)

	//-----------------------------------------------------------------------------------------------
	// Operators
			const	Vector2		operator+( const Vector2& vecToAdd ) const;		// vec2 + vec2
			const	Vector2		operator-( const Vector2& vecToSubtract ) const;	// vec2 - vec2
			const	Vector2		operator*( float uniformScale ) const;			// vec2 * float
			const	Vector2		operator/( float inverseScale ) const;			// vec2 / float
					void		operator+=( const Vector2& vecToAdd );						// vec2 += vec2
					void		operator-=( const Vector2& vecToSubtract );				// vec2 -= vec2
					void		operator*=( const float uniformScale );					// vec2 *= float
					void		operator/=( const float uniformDivisor );					// vec2 /= float
					void		operator=( const Vector2& copyFrom );						// vec2 = vec2
					bool		operator==( const Vector2& compare ) const;				// vec2 == vec2
					bool		operator!=( const Vector2& compare ) const;				// vec2 != vec2
	friend	const	Vector2		operator*( float uniformScale, const Vector2& vecToScale );	// float * vec2

	//-----------------------------------------------------------------------------------------------
	// Accessors and Mutators
					float		GetLength() const;
					float		GetLengthSquared() const ; // faster than GetLength() since it skips the sqrtf()
					float		NormalizeAndGetLength(); // set my new length to 1.0f; keep my direction
					Vector2		GetNormalized() const; // return a new vector, which is a normalized copy of me
					float		GetOrientationDegrees() const; // return 0 for east (5,0), 90 for north (0,8), etc.
	static			Vector2		MakeDirectionAtDegrees( float degrees ); // create vector at angle
					void		SetFromText(const char* text); // Parses vector2 data from string

public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	static const Vector2 ZERO;
	static const Vector2 ONE;
	float x = 0.f;
	float y = 0.f;
};

const Vector2 Interpolate( const Vector2& start, const Vector2& end, float fractionTowardEnd );

// Gets the projected vector in the “projectOnto” direction, whose magnitude is the projected length of “vectorToProject” in that direction.
const Vector2 GetProjectedVector( const Vector2& vectorToProject, const Vector2& projectOnto );

// Returns the vector’s representation/coordinates in (i,j) space (instead of its original x,y space)
const Vector2 GetTransformedIntoBasis( const Vector2& originalVector,
	const Vector2& newBasisI, const Vector2& newBasisJ );

// Takes “vectorInBasis” in (i,j) space and returns the equivalent vector in [axis-aligned] (x,y) Cartesian space
const Vector2 GetTransformedOutOfBasis( const Vector2& vectorInBasis,
	const Vector2& oldBasisI, const Vector2& oldBasisJ );

// Decomposes “originalVector” into two component vectors, which add up to the original:
//   “vectorAlongI” is the vector portion in the “newBasisI” direction, and
//   “vectorAlongJ” is the vector portion in the “newBasisJ” direction.
void DecomposeVectorIntoBasis( const Vector2& originalVector,
	const Vector2& newBasisI, const Vector2& newBasisJ,
	Vector2& out_vectorAlongI, Vector2& out_vectorAlongJ );


const Vector2 Reflect( const Vector2& inVector, const Vector2& normal);


