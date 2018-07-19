#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class Plane
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit Plane( const Vector3& norm, const Vector3& pos );
	explicit Plane( const Vector3& a, const Vector3& b, const Vector3& c );
	~Plane(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	float	GetDistanceFromPlane ( const Vector3& pos ) const;
	bool	IsPointInFront ( const Vector3& pos ) const;
	void	FlipNormal();

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3 normal;
	float distance;
};


