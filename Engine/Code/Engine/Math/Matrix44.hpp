#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
//Forward Declarations

//-----------------------------------------------------------------------------------------------
class Matrix44
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors
	Matrix44(); // default-construct to Identity matrix 
	explicit Matrix44( const float* sixteenValuesBasisMajor ); // Values given as Ix,Iy,Iz,Iw and so on
	explicit Matrix44( const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation = Vector2::ZERO );
	explicit Matrix44( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis,const Vector3& translation = Vector3::ZERO );

	//-----------------------------------------------------------------------------------------------
	// Accessors
	Vector2		TransformPosition2D( const Vector2& position2D ); // Written assuming z=0, w=1
	Vector2		TransformDisplacement2D( const Vector2& displacement2D ); // Written assuming z=0, w=0
	Vector3 TransformDirection3D( const Vector3& direction3D ) const;
	Vector3 TransformPosition3D( const Vector3& position3D ) const;
	Vector3		GetForward() const;
	Vector3		GetRight() const;
	Vector3		GetUp() const;
	Vector3		GetTranslation() const { return Vector3(Tx, Ty, Tz); }
	Vector3		GetEulerAngles() const;
	Matrix44	GetInverse() const;
	float		GetTrace3() const;
	float		GetTrace4() const;

	//-----------------------------------------------------------------------------------------------
	// Mutators
	void		SetIdentity();
	void		SetValues( const float* sixteenValuesBasisMajor ); // float[16] array in order Ix, Iy...
	void		SetFromBasis( const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation );
	void		SetTranslation( const Vector3& translation ); 

private:
	void		SetIBasis( const Vector3& right ); // Can't change from outside
	void		SetJBasis( const Vector3& up );	// Can't change from outside 
	void		SetKBasis( const Vector3& forward ); // Can't change from outside
	 
	//-----------------------------------------------------------------------------------------------
	// Methods
public:
	void		Append( const Matrix44& matrixToAppend ); // a.k.a. Concatenate (right-multiply)
	void		RotateDegrees2D( float rotationDegreesAboutZ ); // 
	void		Translate2D( const Vector2& translation );
	void		ScaleUniform2D( float scaleXY );
	void		Scale2D( float scaleX, float scaleY );
	void		Transpose();
	
	//-----------------------------------------------------------------------------------------------
	// Producers
	static Matrix44 MakeRotationDegrees2D( float rotationDegreesAboutZ );
	static Matrix44 MakeRotationX3D( float rotationX );
	static Matrix44	MakeRotationY3D( float rotationY );
	static Matrix44 MakeRotationZ3D( float rotationZ );
	static Matrix44	MakeRotation3D( const Vector3& rotationXYZ );
	static Matrix44 MakeRotation3D( float x, float y, float z ) { return MakeRotation3D(Vector3(x,y,z)); }
	static Matrix44 MakeTranslation2D( const Vector2& translation );
	static Matrix44 MakeTranslation3D( const Vector3& translation );
	static Matrix44 MakeScaleUniform2D( float scaleXY );
	static Matrix44 MakeScale2D( float scaleX, float scaleY );
	static Matrix44 MakeScale3D( float scaleX, float scaleY, float scaleZ );
	static Matrix44 MakeScale3D( const Vector3& scale );
	static Matrix44 MakeOrtho2D( const Vector2& bottomLeft, const Vector2& topRight );
	static Matrix44 MakeOrtho3D( const Vector2& bottomLeft, const Vector2& topRight, float zNear, float zFar);
	static Matrix44 MakeOrtho3D( float left, float right, float down, float up, float zNear, float zFar);
	static Matrix44	MakePerspectiveMatrix( float fovDegrees, float aspect, float zNear, float zFar);
	static Matrix44 MatrixMultiply( const Matrix44& first, const Matrix44& second );
	static Matrix44 LookAt( const Vector3& position, const Vector3& target, const Vector3& up = Vector3::UP);
	static Matrix44 InvertFast( const Matrix44& mat);
	static Matrix44 Invert( const Matrix44& mat );

	//-----------------------------------------------------------------------------------------------
	// Operators
	void		operator=( const Matrix44& copyFrom );
	bool		operator==( const Matrix44& compare ) const;
	Vector3		operator*( const Vector4& position ) const;
	Matrix44	operator*( const Matrix44& matrix ) const;

	//-----------------------------------------------------------------------------------------------
	// Static members
	static const Matrix44 IDENTITY;

	//-----------------------------------------------------------------------------------------------
	// Members with default initialization
	union
	{
		struct 
		{
			float Ix;
			float Iy;
			float Iz;
			float Iw;
			
			float Jx;
			float Jy;
			float Jz;
			float Jw;

			float Kx;
			float Ky;
			float Kz;
			float Kw;
			
			float Tx;
			float Ty;
			float Tz;
			float Tw;
		};

		float data[16];
	};
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
Matrix44 Interpolate( const Matrix44& start, const Matrix44& end, float fractionTowardEnd );