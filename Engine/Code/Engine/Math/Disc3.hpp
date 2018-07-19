#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class Disc3
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Disc3();
	explicit Disc3( const Vector3& center, float radius );
	explicit Disc3( float x, float y, float z, float radius );
	~Disc3(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	bool IsPointInside( float x, float y, float z ) const ; // is (x,y) within disc’s interior?
	bool IsPointInside( const Vector3& point ) const; // is “point” within disc’s interior?
	void StretchToIncludePoint( float x, float y, float z ); // expand radius if (x,y) is outside
	void StretchToIncludePoint( const Vector3& point );		// expand radius if point outside
	void AddPaddingToRadius( float paddingRadius );
	void Translate( const Vector3& translation ); // move the center
	void Translate( float translationX, float translationY, float translationZ ); // move the center
	
	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3		center;
	float		radius;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
const Disc3 Interpolate( const Disc3& start, const Disc3& end, float fractionTowardEnd );