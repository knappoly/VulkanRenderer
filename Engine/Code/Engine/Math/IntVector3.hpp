#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Vector3;

//-----------------------------------------------------------------------------------------------
class IntVector3
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IntVector3():x(0), y(0), z(0){}
	explicit IntVector3( int newX, int newY, int newZ ):x(newX), y(newY), z(newZ){}
	IntVector3( const Vector3& copyFrom );
	IntVector3( const IntVector3& copyFrom ):x(copyFrom.x), y(copyFrom.y), z(copyFrom.z){}
	~IntVector3(){}
	
	//-----------------------------------------------------------------------------------------------
	// Operators
			const	IntVector3	operator+( const IntVector3& vecToAdd ) const;						// intvec2 + intvec2
			const	IntVector3	operator-( const IntVector3& vecToSubtract ) const;					// intvec2 - intvec2
			const	IntVector3	operator*( int uniformScale ) const;									// intvec2 * int
			const	IntVector3	operator/( int inverseScale ) const;									// intvec2 / int
					void		operator+=( const IntVector3& vecToAdd );									// intvec2 += intvec2
					void		operator-=( const IntVector3& vecToSubtract );								// intvec2 -= intvec2
					void		operator*=( const int uniformScale );										// intvec2 *= int
					void		operator/=( const int uniformDivisor );									// intvec2 /= int
					void		operator=( const  IntVector3& copyFrom );									// intvec2 = intvec2
					bool		operator==( const IntVector3& compare ) const;								// intvec2 == intvec2
					bool		operator!=( const IntVector3& compare ) const;								// intvec2 != intvec2
	friend	const	IntVector3	operator*( int uniformScale, const IntVector3& vecToScale );		// int * intvec2

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
					void		SetFromText(const char* text); // Parses intvector from the string

	//-----------------------------------------------------------------------------------------------
	// Methods
	
	//-----------------------------------------------------------------------------------------------
	// Members
	int x;
	int y;
	int z;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
const IntVector3 Interpolate( const IntVector3& start, const IntVector3& end, float fractionTowardEnd );