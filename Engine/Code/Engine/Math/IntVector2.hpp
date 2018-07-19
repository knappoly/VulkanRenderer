#pragma once
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declaration
class Vector2;
class IntVector3;

//-----------------------------------------------------------------------------------------------
class IntVector2
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IntVector2():x(0), y(0){}
	explicit IntVector2(int newX, int newY):x(newX), y(newY){}
	explicit IntVector2( const Vector2& copyFrom );
	IntVector2( const IntVector2& copyFrom ):x(copyFrom.x), y(copyFrom.y){}
	IntVector2( const IntVector3& copyFrom );
	~IntVector2(){}

	//-----------------------------------------------------------------------------------------------
	// Operators
			const	IntVector2	operator+( const IntVector2& vecToAdd ) const;						// intvec2 + intvec2
			const	IntVector2	operator-( const IntVector2& vecToSubtract ) const;					// intvec2 - intvec2
			const	IntVector2	operator*( int uniformScale ) const;									// intvec2 * int
			const	IntVector2	operator/( int inverseScale ) const;									// intvec2 / int
					void		operator+=( const IntVector2& vecToAdd );									// intvec2 += intvec2
					void		operator-=( const IntVector2& vecToSubtract );								// intvec2 -= intvec2
					void		operator*=( const int uniformScale );										// intvec2 *= int
					void		operator/=( const int uniformDivisor );									// intvec2 /= int
					void		operator=( const  IntVector2& copyFrom );									// intvec2 = intvec2
					bool		operator==( const IntVector2& compare ) const;								// intvec2 == intvec2
					bool		operator!=( const IntVector2& compare ) const;								// intvec2 != intvec2
	friend	const	IntVector2	operator*( int uniformScale, const IntVector2& vecToScale );		// int * intvec2
			

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
					void		SetFromText(const char* text); // Parses intvector from the string
					int			GetLengthSquared() const ; // faster than GetLength() since it skips the sqrtf()
					int			GetLength() const;
					std::string ToString() const;

	//-----------------------------------------------------------------------------------------------
	// Data Members
	int x;
	int y;

	//-----------------------------------------------------------------------------------------------
	// Static constants
	static	const	IntVector2	ZERO;
	static	const	IntVector2	ONE;
};

const IntVector2 Interpolate( const IntVector2& start, const IntVector2& end, float fractionTowardEnd );
