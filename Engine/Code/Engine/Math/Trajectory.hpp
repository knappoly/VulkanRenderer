#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class Trajectory
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Trajectory();
	~Trajectory(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	static	Vector3	GetPositionAtTime( float gravity, float launchSpeed, float launchAngle, float time, const Vector3& direction );
	static	Vector2 Evaluate( float gravity, const Vector2& launch_velocity, float time );
	static	Vector2 Evaluate( float gravity, float launch_speed, float launch_angle, float time );

	static	float	GetMinimumLaunchSpeed( float gravity, float distance );  

	static	bool	GetLaunchAngles( Vector2 *out, 
			float gravity,          // gravity 
			float launch_speed,     // launch speed
			float distance,         // horizontal displacement desired
			float height = 0.0f );   // vertical displacement desired

			float	GetMaxHeight( float gravity, float launch_speed, float distance );

	static	Vector2 GetLaunchVelocity( float gravity,
			float apex_height,      // must be greater than height
			float distance,         // target distance
			float height );         // target height

};

