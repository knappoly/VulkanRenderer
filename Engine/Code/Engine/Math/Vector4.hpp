#pragma once
#pragma warning (disable:4201)
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declaration

//-----------------------------------------------------------------------------------------------
class Vector4
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Vector4(){}
	explicit Vector4( float initialX, float initialY, float initialZ, float initialW ): x(initialX), y(initialY), z(initialZ), w(initialW){}
	explicit Vector4( float initialX, float initialY, float initialZ ): x(initialX), y(initialY), z(initialZ), w(0.f){}
	Vector4( const Vector3& copyFrom, float w = 0.f ); // Construct from vec3 (implicit)
	Vector4( const Vector4& copyFrom ): x(copyFrom.x), y(copyFrom.y), z(copyFrom.z), w(copyFrom.w){} // Copy constructor

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Vector3		xyz() const { return Vector3(x,y,z);	}
	Vector2		xx() const	{ return Vector2(x,x);		}
	Vector2		xy() const	{ return Vector2(x,y);		} 
	Vector2		yy() const	{ return Vector2(y,y);		}
	Vector2		yx() const	{ return Vector2(y,x);		}
	Vector2		zx() const	{ return Vector2(z,x);		}
	Vector2		xz() const	{ return Vector2(x,z);		}

	//-----------------------------------------------------------------------------------------------
	// Methods

	//-----------------------------------------------------------------------------------------------
	// Members
	union 
	{
		struct  
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct  
		{
			float r;
			float g;
			float b;
			float a;
		};

		float data[4];
	};
	
};