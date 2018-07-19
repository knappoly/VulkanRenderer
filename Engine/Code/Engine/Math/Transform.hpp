#pragma once
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/Vector3.hpp"
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations

//-----------------------------------------------------------------------------------------------
struct transform_t 
{
	//-----------------------------------------------------------------------------------------------
	// Constructor
	transform_t()
		: m_position(Vector3::ZERO)
		, m_euler(Vector3::ZERO)
		, m_scale(Vector3::ONE) {}

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Matrix44	GetMatrix() const; 
	void		SetMatrix( const Matrix44& mat );

	Vector3		GetPosition() const { return m_position; } 
	void		SetPosition( const Vector3& pos ) { m_position = pos; }

	void		SetEulerAngles( const Vector3& euler ) { m_euler = euler; } 
	Vector3		GetEulerAngles() const { return m_euler; }

	void		SetScale( const Vector3& scale ) { m_scale = scale; }
	Vector3		GetScale() const { return m_scale; }

	//-----------------------------------------------------------------------------------------------
	// Methods
	void		Translate( const Vector3& offset ) { m_position += offset; }
	void		Translate( float x, float y, float z) { Translate(Vector3(x,y,z)); }
	void		RotateByEuler( const Vector3& euler ) { m_euler += euler; }
	void		RotateByEuler( float x, float y, float z) { RotateByEuler(Vector3(x,y,z)); }

	//-----------------------------------------------------------------------------------------------
	// Members
					Vector3		m_position; 
					Vector3		m_euler; 
					Vector3		m_scale; 

	// Static Members
	static	const	transform_t IDENTITY; 
};

//-----------------------------------------------------------------------------------------------
class Transform
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Transform();
	~Transform(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Vector3		GetForward() const { return m_worldMatrix.GetForward(); }
	Vector3		GetRight() const { return m_worldMatrix.GetRight(); }
	Vector3		GetUp() const { return m_worldMatrix.GetUp(); }
	
	Vector3		GetLocalPosition() const { return m_localTransform.GetPosition(); }
	Vector3		GetWorldPosition() const { return GetWorldMatrix().GetTranslation(); }
	void		SetPosition( const Vector3& newPos );
	void		SetPosition( float x, float y, float z ) { SetPosition(Vector3(x,y,z)); }

	Vector3		GetEulerAngles() const { return m_localTransform.GetEulerAngles(); }
	void		SetEulerAngles( float x, float y, float z ) { SetEulerAngles(Vector3(x,y,z)); }
	void		SetEulerAngles( const Vector3& angles );

	Vector3		GetScale() const { return m_localTransform.GetScale(); }
	void		SetScale( const Vector3& newScale );
	void		SetScale( float x, float y, float z ) { SetScale(Vector3(x,y,z)); }
	void		SetScaleUniform( float xyz ) { SetScale(xyz, xyz, xyz); }

	Matrix44	GetWorldMatrix() const;
	Matrix44	GetLocalMatrix();
	void		SetLocalMatrix( const Matrix44& local );
	void		SetWorldMatrix( const Matrix44& world );

	//-----------------------------------------------------------------------------------------------
	// Methods
	void		RecomputeMatrices( bool isChild = false ) const;
	void		Translate( const Vector3& offset );
	void		Translate( float x, float y, float z) { Translate(Vector3(x,y,z)); }
	void		RotateByEuler( const Vector3& rotation );
	void		RotateByEuler( float x, float y, float z) { RotateByEuler(Vector3(x,y,z)); }
	void		LookAt( const Vector3& worldPosition, const Vector3& worldUp = Vector3::UP );
	void		LocalLookAt( const Vector3& localPosition, const Vector3& localUp = Vector3::UP );
	void		AddChild( Transform* child );
	void		SetDirtyOnHierarchy();
	
	//-----------------------------------------------------------------------------------------------
	// Members
private:
			transform_t				m_localTransform;
	mutable	Matrix44				m_localMatrix;
	mutable	Matrix44				m_worldMatrix;
	mutable	bool					m_isDirty = true;
			Transform*				m_parent = nullptr;
			std::vector<Transform*> m_children;
};

