#pragma once
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class AABB3
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	AABB3(); // Defaults to negative and positive infinity
	explicit AABB3( const Vector3& newMins, const Vector3& newMaxs ):mins(newMins), maxs(newMaxs) { }
	~AABB3(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	bool	IsInvalid();
	Vector3	GetBackBottomLeft();
	Vector3	GetBackBottomRight();
	Vector3 GetBackTopRight();
	Vector3 GetBackTopLeft();
	Vector3 GetFrontBottomLeft();
	Vector3 GetFrontBottomRight();
	Vector3 GetFrontTopRight();
	Vector3 GetFrontTopLeft();
	Vector3	GetCenter();
	Vector3 GetHalfExtents();
	Vector3	GetSizes();

	//-----------------------------------------------------------------------------------------------
	// Methods
	void	SetCenter( const Vector3& center );
	void	Translate( const Vector3& translation );
	void	GrowToContain( const Vector3& pos );
	bool	IsPointInside( const Vector3& pos ) const;

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3 mins;
	Vector3 maxs;
};



