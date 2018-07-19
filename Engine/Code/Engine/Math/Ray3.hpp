#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class Ray3
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Ray3(): start(Vector3::ZERO), dir(Vector3::ZERO) {}
	explicit Ray3(const Vector3& startPoint, const Vector3& direction ):start(startPoint), dir(direction){};
	~Ray3(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	bool	IsInvalid() const { return dir == Vector3::ZERO; }

	//-----------------------------------------------------------------------------------------------
	// Methods
	Vector3	Evaluate( float t ) const;

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3	start;
	Vector3 dir;
};

