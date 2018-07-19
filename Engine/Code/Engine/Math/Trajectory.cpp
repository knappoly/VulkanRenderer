#include "Engine/Math/Trajectory.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor - DUMMY
//
Trajectory::Trajectory()
{

}

//-----------------------------------------------------------------------------------------------
// Gets the 3D position at time 
//
Vector3 Trajectory::GetPositionAtTime(float gravity, float launchSpeed, float launchAngle, float time, const Vector3& direction)
{
	// Evaluate and get the 2d position
	Vector2 position2D = Evaluate(gravity, launchSpeed, launchAngle, time);

	// Use direction vector to get the rotation part
	float rotation = Atan2Degrees(direction.xz().y, direction.xz().x); // Y is the Z coordinate in this case
	
	// Determine 3D position by using Polar coords for the XZ plane
	return Vector3(position2D.x * CosDegrees(rotation), position2D.y, position2D.x * SinDegrees(rotation)); 
}

//-----------------------------------------------------------------------------------------------
// Evaluates the curve and gives a point at a given time
//
Vector2 Trajectory::Evaluate(float gravity, const Vector2& launch_velocity, float time)
{
	Vector2 gravityVec(0.f, gravity);
	Vector2 resultant = (launch_velocity * time) - (0.5f * gravityVec * time * time);
	
	return resultant;
}

//-----------------------------------------------------------------------------------------------
// Evaluates the curve and gives a point at a given time
//
Vector2 Trajectory::Evaluate(float gravity, float launch_speed, float launch_angle, float time)
{
	Vector2 velocityVec = Vector2::MakeDirectionAtDegrees(launch_angle) * launch_speed;

	return Evaluate(gravity, velocityVec, time);
}

//-----------------------------------------------------------------------------------------------
// Returns the minimum launch speed
//
float Trajectory::GetMinimumLaunchSpeed(float gravity, float distance)
{
	return sqrtf(gravity * distance);
}

//-----------------------------------------------------------------------------------------------
// Returns the launch angles 
//
bool Trajectory::GetLaunchAngles(Vector2 *out, float gravity, float launch_speed, float distance, float height /*= 0.0f */)
{
	float speedSquared = launch_speed * launch_speed;

// 	if(height == 0.f)
// 	{
// 		// Single root . No height difference. Simpler calculation
// 		float value = (-1.f * gravity * distance) / speedSquared;
// 		out->x = 0.5f * AsinDegress(value);
// 		out->y = out->x;
// 		return true;
// 	}

	float distSquared = distance * distance;

	float deter = (speedSquared * speedSquared) - (gravity * ( (gravity * distSquared) + (2.f * height * speedSquared)));

	float denominator = distance * gravity;

	if(deter < 0.f)
	{
		return false; // No roots because determinant is negative
	}

	deter = sqrtf(deter);
	out->x = Atan2Degrees((speedSquared + deter), denominator); // First root

	if(deter == 0.f)
	{
		out->y = out->x; // Both roots are equal if determinant is equal to 0
	}
	else
	{
		out->y = 90.f - out->x; // Both distinct roots
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
// Returns the maximum height that's reached with the two angles
//
float Trajectory::GetMaxHeight(float gravity, float launch_speed, float distance)
{
	Vector2 angles;
	GetLaunchAngles(&angles, gravity, launch_speed, distance);

	float maxHeight = (launch_speed * SinDegrees(angles.x) * 0.5f ) * 1.f / gravity;
	return maxHeight;
}

//-----------------------------------------------------------------------------------------------
// Returns the launch velocity after computation
//
Vector2 Trajectory::GetLaunchVelocity(float gravity, float apex_height, float distance, float height)
{
	Vector2 velocity;

	if( apex_height < height )
	{
		ConsolePrintf(Rgba::RED, "WRONG DATA. APEX HEIGHT SHOULD BE HIGHER THAN HEIGHT");
	}

	Vector2 time; // Single value only wwill be used but quadratic function takes Vector2

	velocity.y = sqrtf(2.f * gravity * apex_height);

	float a = -0.5f * gravity;
	float b = velocity.y;
	float c = height;
	Quadratic( &time, a, b, c );

	float angle = Atan2Degrees((( velocity.y - ( gravity * time.x )) * time.x), distance);
	float cosTheta =  CosDegrees(angle);
	float launchSpeed = distance * 1.f / (time.x * cosTheta);

	velocity.x = launchSpeed * cosTheta;
	return velocity;
}
