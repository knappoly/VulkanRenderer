#pragma once
#include "Engine/Math/Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class OBB3
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	OBB3(){}
	explicit OBB3( const Matrix44& spaceMat );
	explicit OBB3( const Vector3& center, const Vector3& right = Vector3::RIGHT, const Vector3& up = Vector3::UP, const Vector3& forward = Vector3::FORWARD );
	~OBB3(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void	SetSpace( const Matrix44& spaceMat );
	void	SetCenter( const Vector3& center );
	void	SetSpace( const Vector3& right, const Vector3& up, const Vector3& forward );

	//-----------------------------------------------------------------------------------------------
	// Methods
	bool	DoesContaintPoint( const Vector3& point );
	void	GetCorners( Vector3* out );
	
	//-----------------------------------------------------------------------------------------------
	// Members
private:
	Matrix44 space;
	Matrix44 inverseSpace;
};

