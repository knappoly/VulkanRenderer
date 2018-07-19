#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vector4.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
const Matrix44 Matrix44::IDENTITY;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Matrix44::Matrix44()
{
	Ix = 1;
	Iy = 0;
	Iz = 0;
	Iw = 0;

	Jx = 0;
	Jy = 1;
	Jz = 0;
	Jw = 0;

	Kx = 0;
	Ky = 0;
	Kz = 1;
	Kw = 0;

	Tx = 0;
	Ty = 0;
	Tz = 0;
	Tw = 1;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Matrix44::Matrix44( const float* sixteenValuesBasisMajor )
{
	Ix = sixteenValuesBasisMajor[0];
	Iy = sixteenValuesBasisMajor[1];
	Iz = sixteenValuesBasisMajor[2];
	Iw = sixteenValuesBasisMajor[3];

	Jx = sixteenValuesBasisMajor[4]; 
	Jy = sixteenValuesBasisMajor[5];
	Jz = sixteenValuesBasisMajor[6];
	Jw = sixteenValuesBasisMajor[7];

	Kx = sixteenValuesBasisMajor[8];
	Ky = sixteenValuesBasisMajor[9];
	Kz = sixteenValuesBasisMajor[10];
	Kw = sixteenValuesBasisMajor[11];

	Tx = sixteenValuesBasisMajor[12];
	Ty = sixteenValuesBasisMajor[13];
	Tz = sixteenValuesBasisMajor[14];
	Tw = sixteenValuesBasisMajor[15];
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Matrix44::Matrix44( const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation )
{
	Ix = iBasis.x;
	Iy = iBasis.y;
	Iz = 0.f;
	Iw = 0.f;

	Jx = jBasis.x;
	Jy = jBasis.y;
	Jz = 0.f;
	Jw = 0.f;

	Kx = 0.f;
	Ky = 0.f;
	Kz = 1.f;
	Kw = 0.f;

	Tx = translation.x;
	Ty = translation.y;
	Tz = 0.f;
	Tw = 1.f;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Matrix44::Matrix44(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation /*= Vector3::ZERO */)
{
	Ix = iBasis.x;
	Iy = iBasis.y;
	Iz = iBasis.z;
	Iw = 0.f;

	Jx = jBasis.x;
	Jy = jBasis.y;
	Jz = jBasis.z;
	Jw = 0.f;

	Kx = kBasis.x;
	Ky = kBasis.y;
	Kz = kBasis.z;
	Kw = 0.f;

	Tx = translation.x;
	Ty = translation.y;
	Tz = translation.z;
	Tw = 1.f;
}

//-----------------------------------------------------------------------------------------------
// Applies the transformation to the position and returns the world-position
//
Vector2 Matrix44::TransformPosition2D(const Vector2& position2D)
{
	Vector2 newPosition2D;
	newPosition2D.x = (Ix * position2D.x) + (Jx * position2D.y) + Tx;
	newPosition2D.y = (Iy * position2D.x) + (Jy * position2D.y) + Ty;
	return newPosition2D;
}

//-----------------------------------------------------------------------------------------------
// Normalizes and returns the forward vector
//
Vector3 Matrix44::GetForward() const
{
	return Vector3(Kx,Ky,Kz).GetNormalized();
}

//-----------------------------------------------------------------------------------------------
// Normalizes and returns the right vector
//
Vector3 Matrix44::GetRight() const
{
	return Vector3(Ix,Iy,Iz).GetNormalized();
}

//-----------------------------------------------------------------------------------------------
// Normalizes and returns the up vector
//
Vector3 Matrix44::GetUp() const
{
	return Vector3(Jx,Jy,Jz).GetNormalized();
}

//-----------------------------------------------------------------------------------------------
// Applies the transformation to the displacement vector and returns the world-displacement
//
Vector2 Matrix44::TransformDisplacement2D(const Vector2& displacement2D)
{
	Vector2 newDisplacement2D;
	newDisplacement2D.x = (Ix * displacement2D.x) + (Jx * displacement2D.y);
	newDisplacement2D.y = (Iy * displacement2D.x) + (Jy * displacement2D.y);
	return newDisplacement2D;
}

//-----------------------------------------------------------------------------------------------
// Applies the transformation to the displacement vector and returns the world-displacement
//
Vector3 Matrix44::TransformDirection3D(const Vector3& direction3D) const
{
	Vector3 newDisplacement3D;
	newDisplacement3D.x = (Ix * direction3D.x) + (Jx * direction3D.y) + (Kx * direction3D.z);
	newDisplacement3D.y = (Iy * direction3D.x) + (Jy * direction3D.y) + (Ky * direction3D.z);
	newDisplacement3D.z = (Iz * direction3D.x) + (Jz * direction3D.y) + (Kz * direction3D.z);
	return newDisplacement3D;
}

//-----------------------------------------------------------------------------------------------
// Applies the transformation to the displacement vector and returns the world-displacement
//
Vector3 Matrix44::TransformPosition3D(const Vector3& position3D) const
{
	Vector3 newPosition3D;
	newPosition3D.x = (Ix * position3D.x) + (Jx * position3D.y) + (Kx * position3D.z) + Tx;
	newPosition3D.y = (Iy * position3D.x) + (Jy * position3D.y) + (Ky * position3D.z) + Ty;
	newPosition3D.z = (Iz * position3D.x) + (Jz * position3D.y) + (Kz * position3D.z) + Tz;
	return newPosition3D;
}

//-----------------------------------------------------------------------------------------------
// Returns the angles from the matrix
//
Vector3 Matrix44::GetEulerAngles() const
{
	float xDeg = -1.f * asinf(Ky);
	float yDeg = 0.f;
	float zDeg = 0.f;;

	xDeg = ClampFloatNegativeOneToOne(xDeg);

	float cx = acosf(xDeg);
	if(cx != 0.f)
	{
		yDeg = atan2f(Kx, Kz);
		zDeg = atan2f(Iy, Jy);
	}
	else
	{
		// Gimble lock case
		zDeg = 0.f;
		yDeg = atan2f(-Kx, Ix);
	}
	xDeg = ConvertRadiansToDegrees(xDeg);
	yDeg = ConvertRadiansToDegrees(yDeg);
	zDeg = ConvertRadiansToDegrees(zDeg);

	return Vector3(xDeg, yDeg, zDeg);
}

//-----------------------------------------------------------------------------------------------
// Returns the inverse of this matrix
//
Matrix44 Matrix44::GetInverse() const
{
	Matrix44 inverse = Matrix44::Invert(*this);

	return inverse;
}

//-----------------------------------------------------------------------------------------------
// Returns the trace of the matrix xyz i.e. sum of primary diagonal
//
float Matrix44::GetTrace3() const
{
	return (Ix + Jy + Kz);
}

//-----------------------------------------------------------------------------------------------
// Returns the trace of the matrix xyzw i.e. sum of primary diagonal
//
float Matrix44::GetTrace4() const
{
	return (Ix + Jy + Kz + Tw);
}

//-----------------------------------------------------------------------------------------------
// Sets the matrix to identity matrix
//
void Matrix44::SetIdentity()
{
	Ix = 1.f;
	Iy = 0.f;
	Iz = 0.f;
	Iw = 0.f;
	Jx = 0.f;
	Jy = 1.f;
	Jz = 0.f;
	Jw = 0.f;
	Kx = 0.f;
	Ky = 0.f;
	Kz = 1.f;
	Kw = 0.f;
	Tx = 0.f;
	Ty = 0.f;
	Tz = 0.f;
	Tw = 1.f;
}

//-----------------------------------------------------------------------------------------------
// Sets the values of the matrix (Input: Basis major)
//
void Matrix44::SetValues( const float* sixteenValuesBasisMajor )
{
	Ix = sixteenValuesBasisMajor[0];
	Iy = sixteenValuesBasisMajor[1];
	Iz = sixteenValuesBasisMajor[2];
	Iw = sixteenValuesBasisMajor[3];

	Jx = sixteenValuesBasisMajor[4]; 
	Jy = sixteenValuesBasisMajor[5];
	Jz = sixteenValuesBasisMajor[6];
	Jw = sixteenValuesBasisMajor[7];

	Kx = sixteenValuesBasisMajor[8];
	Ky = sixteenValuesBasisMajor[9];
	Kz = sixteenValuesBasisMajor[10];
	Kw = sixteenValuesBasisMajor[11];

	Tx = sixteenValuesBasisMajor[12];
	Ty = sixteenValuesBasisMajor[13];
	Tz = sixteenValuesBasisMajor[14];
	Tw = sixteenValuesBasisMajor[15];
}

//-----------------------------------------------------------------------------------------------
// Sets the matrix from given basis
//
void Matrix44::SetFromBasis(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation)
{
	SetIBasis(right);
	SetJBasis(up);
	SetKBasis(forward);
	SetTranslation(translation);
}

//-----------------------------------------------------------------------------------------------
// Sets the I Basis on the matrix
//
void Matrix44::SetIBasis(const Vector3& right)
{
	Ix = right.x;
	Iy = right.y;
	Iz = right.z;
}

//-----------------------------------------------------------------------------------------------
// Sets the J Basis on the matrix
//
void Matrix44::SetJBasis(const Vector3& up)
{
	Jx = up.x;
	Jy = up.y;
	Jz = up.z;
}

//-----------------------------------------------------------------------------------------------
// Sets the J Basis on the matrix
//
void Matrix44::SetKBasis(const Vector3& forward)
{
	Kx = forward.x;
	Ky = forward.y;
	Kz = forward.z;
}

//-----------------------------------------------------------------------------------------------
// Sets the translation part on the matrix
//
void Matrix44::SetTranslation(const Vector3& translation)
{
	Tx = translation.x;
	Ty = translation.y;
	Tz = translation.z;
}

//-----------------------------------------------------------------------------------------------
// Appends the matrix to the right of the existing matrix (Right multiply)
//
void Matrix44::Append( const Matrix44& matrixToAppend )
{
	Matrix44 oldMatrix = *this;
	*this = Matrix44::MatrixMultiply( oldMatrix, matrixToAppend );
}

//-----------------------------------------------------------------------------------------------
// Rotates the matrix
//
void Matrix44::RotateDegrees2D(float rotationDegreesAboutZ)
{
	Append(Matrix44::MakeRotationDegrees2D(rotationDegreesAboutZ));
}

//-----------------------------------------------------------------------------------------------
// Translates the matrix
//
void Matrix44::Translate2D(const Vector2& translation)
{
	Append(Matrix44::MakeTranslation2D(translation));
}

//-----------------------------------------------------------------------------------------------
// Scales the matrix uniformly
//
void Matrix44::ScaleUniform2D(float scaleXY)
{
	Append(Matrix44::MakeScaleUniform2D(scaleXY));
}

//-----------------------------------------------------------------------------------------------
// Scales the matrix
// 
void Matrix44::Scale2D(float scaleX, float scaleY)
{
	Append(Matrix44::MakeScale2D(scaleX,scaleY));
}

//-----------------------------------------------------------------------------------------------
// Transposes the matrix
//
void Matrix44::Transpose()
{
	float values[] = {	Ix, Jx, Kx, Tx,     // Following Basis major. This is just float value order
						Iy, Jy, Ky, Ty, 
						Iz, Jz, Kz, Tz,
						Iw, Jw, Kw, Tw};
	*this = Matrix44(values);
}

//-----------------------------------------------------------------------------------------------
// Returns a rotation matrix
//
Matrix44 Matrix44::MakeRotationDegrees2D(float rotationDegreesAboutZ)
{
	Matrix44 rotationMatrix;
	if(fmodf(rotationDegreesAboutZ, 360.f) != 0.f)
	{
		float cosValue = CosDegrees(rotationDegreesAboutZ);
		float sinValue = SinDegrees(rotationDegreesAboutZ);

		rotationMatrix.Ix = cosValue;
		rotationMatrix.Iy = sinValue;

		rotationMatrix.Jx = -sinValue;
		rotationMatrix.Jy = cosValue;
	}

	return rotationMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a rotation matrix around X axis
//
Matrix44 Matrix44::MakeRotationX3D(float rotationX)
{
	Matrix44 rotationMatrix;
	if(fmodf(rotationX, 360.f) != 0.f)
	{
		float cosValue = CosDegrees(rotationX);
		float sinValue = SinDegrees(rotationX);

		rotationMatrix.Jy = cosValue;
		rotationMatrix.Jz = sinValue;

		rotationMatrix.Ky = -sinValue;
		rotationMatrix.Kz = cosValue;
	}

	return rotationMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a rotation matrix around Y axis
//
Matrix44 Matrix44::MakeRotationY3D(float rotationY)
{
	Matrix44 rotationMatrix;
	if(fmodf(rotationY, 360.f) != 0.f)
	{
		float cosValue = CosDegrees(rotationY);
		float sinValue = SinDegrees(rotationY);

		rotationMatrix.Ix = cosValue;
		rotationMatrix.Iz = -sinValue;

		rotationMatrix.Kx = sinValue;
		rotationMatrix.Kz = cosValue;
	}
	
	return rotationMatrix;
}

//-----------------------------------------------------------------------------------------------
// Makes rotation matrix along Z axis
//
Matrix44 Matrix44::MakeRotationZ3D(float rotationZ)
{
	return MakeRotationDegrees2D(rotationZ);
}

//-----------------------------------------------------------------------------------------------
// Makes a rotation matrix from rotation around x,y,z
//
Matrix44 Matrix44::MakeRotation3D(const Vector3& rotationXYZ)
{
	float cx = CosDegrees(rotationXYZ.x);
	float sx = SinDegrees(rotationXYZ.x);

	float cy = CosDegrees(rotationXYZ.y);
	float sy = SinDegrees(rotationXYZ.y); 

	float cz = CosDegrees(rotationXYZ.z);
	float sz = SinDegrees(rotationXYZ.z);

	float values[] = { 
		 cz*cy + sz*sx*sy,		 sz*cx,		-cz*sy + sz*sx*cy,		0,	
		-sz*cy + cz*sx*sy,		 cz*cx,		 sz*sy + cz*sx*cy,		0,	
		 cx*sy,					-sx,		 cx*cy,					0,
		  0,					 0,			 0,						1 }; // Actually column major , this is just data order

	Matrix44 rotationMatrix(values);
	
	return rotationMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a translation matrix
//
Matrix44 Matrix44::MakeTranslation2D(const Vector2& translation)
{
	Matrix44 translationMatrix;

	translationMatrix.Tx = translation.x;
	translationMatrix.Ty = translation.y;

	return translationMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a 3D translation matrix
//
Matrix44 Matrix44::MakeTranslation3D(const Vector3& translation)
{
	Matrix44 translationMatrix;

	translationMatrix.Tx = translation.x;
	translationMatrix.Ty = translation.y;
	translationMatrix.Tz = translation.z;

	return translationMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a scale uniform matrix
//
Matrix44 Matrix44::MakeScaleUniform2D(float scaleXY)
{
	Matrix44 scaleMatrix;

	scaleMatrix.Ix = scaleXY;
	scaleMatrix.Jy = scaleXY;

	return scaleMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a scale matrix
//
Matrix44 Matrix44::MakeScale2D(float scaleX, float scaleY)
{
	Matrix44 scaleMatrix;

	scaleMatrix.Ix = scaleX;
	scaleMatrix.Jy = scaleY;

	return scaleMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns a scale matrix
//
Matrix44 Matrix44::MakeScale3D(const Vector3& scale)
{
	return MakeScale3D(scale.x, scale.y, scale.z);
}

//-----------------------------------------------------------------------------------------------
// Returns a scale matrix
//
Matrix44 Matrix44::MakeScale3D(float scaleX, float scaleY, float scaleZ)
{
	Matrix44 scaleMatrix;

	scaleMatrix.Ix = scaleX;
	scaleMatrix.Jy = scaleY;
	scaleMatrix.Kz = scaleZ;

	return scaleMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns an ortho projection matrix
//
Matrix44 Matrix44::MakeOrtho2D(const Vector2& bottomLeft, const Vector2& topRight)
{
	return MakeOrtho3D(bottomLeft, topRight, 0.f, 1.f);
}

//-----------------------------------------------------------------------------------------------
// Returns a 3D Ortho projection matrix
//
Matrix44 Matrix44::MakeOrtho3D(const Vector2& bottomLeft, const Vector2& topRight, float zNear, float zFar)
{
	return MakeOrtho3D(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, zNear, zFar);
}

//-----------------------------------------------------------------------------------------------
// Returns a 3D Ortho projection matrix
//
Matrix44 Matrix44::MakeOrtho3D(float left, float right, float down, float up, float zNear, float zFar)
{
	Matrix44 projMatrix; 
	projMatrix.Ix = 2.f / (right - left);
	projMatrix.Jy = 2.f / (up - down); 
	projMatrix.Kz = 2.f / (zFar - zNear);

	projMatrix.Tx = -1.f * (right + left) / (right - left);
	projMatrix.Ty = -1.f * (up + down) / (up - down);
	projMatrix.Tz = -1.f * (zFar + zNear) / (zFar - zNear);
	projMatrix.Tw = 1;

	return projMatrix;
}

//-----------------------------------------------------------------------------------------------
// Computes a Perspective projection matrix
//
Matrix44 Matrix44::MakePerspectiveMatrix(float fovDegrees, float aspect, float zNear, float zFar)
{
	Matrix44 projMatrix; 
	float screenDist;
	float distPrime;

	if (zNear == 0.f) // zNear cannot be zero
	{
		zNear = 0.1f;
	}

	float tanThetaHalf = TanDegrees(fovDegrees * 0.5f);
	screenDist = 1.f / tanThetaHalf;

	if(aspect > 1.f)
	{
		distPrime = screenDist / aspect;
	}
	else
	{
		distPrime = screenDist * aspect;
	}

	float kz = (zNear + zFar) * 1.f / (zFar - zNear);
	float tz = (-2.f * zNear * zFar) * 1.f / (zFar - zNear);

	projMatrix.Ix = distPrime;
	projMatrix.Jy = screenDist;
	projMatrix.Kz = kz;
	projMatrix.Kw = 1;
	projMatrix.Tz = tz;
	projMatrix.Tw = 0.f;

	return projMatrix;
}

//-----------------------------------------------------------------------------------------------
// Returns the result matrix after multiplying the matrices
//
Matrix44 Matrix44::MatrixMultiply(const Matrix44& first, const Matrix44& second)
{
	Matrix44 result;

	result.Ix = (first.Ix * second.Ix) + (first.Jx * second.Iy) + (first.Kx * second.Iz) + (first.Tx * second.Iw);
	result.Iy = (first.Iy * second.Ix) + (first.Jy * second.Iy) + (first.Ky * second.Iz) + (first.Ty * second.Iw);
	result.Iz = (first.Iz * second.Ix) + (first.Jz * second.Iy) + (first.Kz * second.Iz) + (first.Tz * second.Iw);
	result.Iw = (first.Iw * second.Ix) + (first.Jw * second.Iy) + (first.Kw * second.Iz) + (first.Tw * second.Iw);

	result.Jx = (first.Ix * second.Jx) + (first.Jx * second.Jy) + (first.Kx * second.Jz) + (first.Tx * second.Jw);
	result.Jy = (first.Iy * second.Jx) + (first.Jy * second.Jy) + (first.Ky * second.Jz) + (first.Ty * second.Jw);
	result.Jz = (first.Iz * second.Jx) + (first.Jz * second.Jy) + (first.Kz * second.Jz) + (first.Tz * second.Jw);
	result.Jw = (first.Iw * second.Jx) + (first.Jw * second.Jy) + (first.Kw * second.Jz) + (first.Tw * second.Jw);

	result.Kx = (first.Ix * second.Kx) + (first.Jx * second.Ky) + (first.Kx * second.Kz) + (first.Tx * second.Kw);
	result.Ky = (first.Iy * second.Kx) + (first.Jy * second.Ky) + (first.Ky * second.Kz) + (first.Ty * second.Kw);
	result.Kz = (first.Iz * second.Kx) + (first.Jz * second.Ky) + (first.Kz * second.Kz) + (first.Tz * second.Kw);
	result.Kw = (first.Iw * second.Kx) + (first.Jw * second.Ky) + (first.Kw * second.Kz) + (first.Tw * second.Kw);

	result.Tx = (first.Ix * second.Tx) + (first.Jx * second.Ty) + (first.Kx * second.Tz) + (first.Tx * second.Tw);
	result.Ty = (first.Iy * second.Tx) + (first.Jy * second.Ty) + (first.Ky * second.Tz) + (first.Ty * second.Tw);
	result.Tz = (first.Iz * second.Tx) + (first.Jz * second.Ty) + (first.Kz * second.Tz) + (first.Tz * second.Tw);
	result.Tw = (first.Iw * second.Tx) + (first.Jw * second.Ty) + (first.Kw * second.Tz) + (first.Tw * second.Tw);

	return result;
}

//-----------------------------------------------------------------------------------------------
// Returns a look at matrix with position as the eye
//
Matrix44 Matrix44::LookAt(const Vector3& position, const Vector3& target, const Vector3& up /*= Vector3::UP*/)
{
	Matrix44 look;

	if(position == target)
	{
		return look;
	}

	Vector3 dir = target - position;
	Vector3 forward = dir.GetNormalized();

	Vector3 right = CrossProduct(up, forward);
	right.Normalize();

	Vector3 correctUp = CrossProduct(forward, right);
	correctUp.Normalize();
	
	look = Matrix44(right, correctUp, forward, position);
	return look;
}

//-----------------------------------------------------------------------------------------------
// Inverts the matrix by transposing it (Assumes: Orthonormal)
//
Matrix44 Matrix44::InvertFast(const Matrix44& mat)
{
	// Transpose the rotational part. Create a translation matrix with [-t] values and append it to get the inverse
	Matrix44 rotationTranspose = mat;
	rotationTranspose.Tx = rotationTranspose.Ty = rotationTranspose.Tz = 0.f;
	rotationTranspose.Transpose();

//  	Matrix44 translationTranspose = Matrix44::MakeTranslation3D(Vector3(-mat.Tx, -mat.Ty, -mat.Tz));
// 	rotationTranspose.Append(translationTranspose);

	Vector3 translation(-mat.Tx, -mat.Ty, -mat.Tz);

	rotationTranspose.Tx = DotProduct(translation, mat.GetRight());
	rotationTranspose.Ty = DotProduct(translation, mat.GetUp());
	rotationTranspose.Tz = DotProduct(translation, mat.GetForward());

	Matrix44 inverted = rotationTranspose;
	return inverted;
}

//-----------------------------------------------------------------------------------------------
// Does a full inverse operation on the mat and returns the result
//
Matrix44 Matrix44::Invert(const Matrix44& mat)
{
	double inv[16];
	double det;
	double m[16];
	uint index;

	for (index = 0; index < 16; ++index) {
		m[index] = (double) mat.data[index];
	}

	inv[0] = m[5]  * m[10] * m[15] - 
		m[5]  * m[11] * m[14] - 
		m[9]  * m[6]  * m[15] + 
		m[9]  * m[7]  * m[14] +
		m[13] * m[6]  * m[11] - 
		m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] + 
		m[4]  * m[11] * m[14] + 
		m[8]  * m[6]  * m[15] - 
		m[8]  * m[7]  * m[14] - 
		m[12] * m[6]  * m[11] + 
		m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] - 
		m[4]  * m[11] * m[13] - 
		m[8]  * m[5] * m[15] + 
		m[8]  * m[7] * m[13] + 
		m[12] * m[5] * m[11] - 
		m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] + 
		m[4]  * m[10] * m[13] +
		m[8]  * m[5] * m[14] - 
		m[8]  * m[6] * m[13] - 
		m[12] * m[5] * m[10] + 
		m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] + 
		m[1]  * m[11] * m[14] + 
		m[9]  * m[2] * m[15] - 
		m[9]  * m[3] * m[14] - 
		m[13] * m[2] * m[11] + 
		m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] - 
		m[0]  * m[11] * m[14] - 
		m[8]  * m[2] * m[15] + 
		m[8]  * m[3] * m[14] + 
		m[12] * m[2] * m[11] - 
		m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] + 
		m[0]  * m[11] * m[13] + 
		m[8]  * m[1] * m[15] - 
		m[8]  * m[3] * m[13] - 
		m[12] * m[1] * m[11] + 
		m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] - 
		m[0]  * m[10] * m[13] - 
		m[8]  * m[1] * m[14] + 
		m[8]  * m[2] * m[13] + 
		m[12] * m[1] * m[10] - 
		m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] - 
		m[1]  * m[7] * m[14] - 
		m[5]  * m[2] * m[15] + 
		m[5]  * m[3] * m[14] + 
		m[13] * m[2] * m[7] - 
		m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] + 
		m[0]  * m[7] * m[14] + 
		m[4]  * m[2] * m[15] - 
		m[4]  * m[3] * m[14] - 
		m[12] * m[2] * m[7] + 
		m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] - 
		m[0]  * m[7] * m[13] - 
		m[4]  * m[1] * m[15] + 
		m[4]  * m[3] * m[13] + 
		m[12] * m[1] * m[7] - 
		m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] + 
		m[0]  * m[6] * m[13] + 
		m[4]  * m[1] * m[14] - 
		m[4]  * m[2] * m[13] - 
		m[12] * m[1] * m[6] + 
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + 
		m[1] * m[7] * m[10] + 
		m[5] * m[2] * m[11] - 
		m[5] * m[3] * m[10] - 
		m[9] * m[2] * m[7] + 
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - 
		m[0] * m[7] * m[10] - 
		m[4] * m[2] * m[11] + 
		m[4] * m[3] * m[10] + 
		m[8] * m[2] * m[7] - 
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + 
		m[0] * m[7] * m[9] + 
		m[4] * m[1] * m[11] - 
		m[4] * m[3] * m[9] - 
		m[8] * m[1] * m[7] + 
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - 
		m[0] * m[6] * m[9] - 
		m[4] * m[1] * m[10] + 
		m[4] * m[2] * m[9] + 
		m[8] * m[1] * m[6] - 
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	det = 1.0 / det;

	Matrix44 inverseMatrix;
	for (index = 0; index < 16; index++) {
		inverseMatrix.data[index] = (float)(inv[index] * det);
	}

	return inverseMatrix;
}

//-----------------------------------------------------------------------------------------------
// Operator overload
//
void Matrix44::operator=(const Matrix44& copyFrom)
{
	float values[] = {copyFrom.Ix, copyFrom.Iy, copyFrom.Iz, copyFrom.Iw,     // Following Basis major. This is just float value order
					  copyFrom.Jx,copyFrom.Jy,  copyFrom.Jz, copyFrom.Jw, 
					  copyFrom.Kx, copyFrom.Ky, copyFrom.Kz, copyFrom.Kw,
					  copyFrom.Tx, copyFrom.Ty, copyFrom.Tz, copyFrom.Tw};
	SetValues(values);
}

//-----------------------------------------------------------------------------------------------
// Operator ==
//
bool Matrix44::operator==(const Matrix44& compare) const
{
	if(
		Ix == compare.Ix &&
		Iy == compare.Iy &&
		Iz == compare.Iz && 
		Iw == compare.Iw &&
		Jx == compare.Jx &&
		Jy == compare.Jy &&
		Jz == compare.Jz &&
		Jw == compare.Jw &&
		Kx == compare.Kx &&
		Ky == compare.Ky &&
		Kz == compare.Kz &&
		Kw == compare.Kw &&
		Tx == compare.Tx &&	
		Ty == compare.Ty &&
		Tz == compare.Tz &&
		Tw == compare.Tw)
	{
		return true;
	}

	else
	{
		return false;
	}
}

//-----------------------------------------------------------------------------------------------
// Operator * . Transforms the point
//
Vector3 Matrix44::operator*(const Vector4& position) const
{
	Vector3 newPos = (position.w == 1) ? this->TransformPosition3D(position.xyz()) : this->TransformDirection3D(position.xyz());
	return newPos;
}

//-----------------------------------------------------------------------------------------------
// Operator *. Matrix multiply
//
Matrix44 Matrix44::operator*(const Matrix44& matrix) const
{
	return MatrixMultiply(*this, matrix);
}

//-----------------------------------------------------------------------------------------------
// Interpolates between the matrices
//
Matrix44 Interpolate(const Matrix44& start, const Matrix44& end, float fractionTowardEnd)
{
	Vector3 aRight = start.GetRight();
	Vector3 bRight = end.GetRight(); 
	Vector3 aUp = start.GetUp();
	Vector3 bUp = end.GetUp(); 
	Vector3 aForward = start.GetForward(); 
	Vector3 bForward = end.GetForward();
	Vector3 aTranslation = start.GetTranslation();
	Vector3 bTranslation = end.GetTranslation(); 

	Vector3 right = Slerp( aRight, bRight, fractionTowardEnd ); 
	Vector3 up = Slerp( aUp, bUp, fractionTowardEnd ); 
	Vector3 forward = Slerp( aForward, bForward, fractionTowardEnd ); 
	Vector3 translation = Interpolate( aTranslation, bTranslation, fractionTowardEnd ); 

	return Matrix44( right, up, forward, translation ); 
}
