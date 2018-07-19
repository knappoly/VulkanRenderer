#include "Engine/Math/Transform.hpp"
#include "Engine/Math/Vector4.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
const transform_t transform_t::IDENTITY;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Transform::Transform()
{

}

//-----------------------------------------------------------------------------------------------
// Computes the world matrix on demand if its dirty
//
Matrix44 Transform::GetWorldMatrix() const
{
	if(m_isDirty)
	{
		RecomputeMatrices(m_parent);
		m_isDirty = false;
	}

	return m_worldMatrix;
}

//-----------------------------------------------------------------------------------------------
// Computes the local matrix on demand if its dirty and if its not parent
//
Matrix44 Transform::GetLocalMatrix()
{
	if(!m_parent)
	{
		m_localMatrix = GetWorldMatrix();
		return m_localMatrix;
	}

	else
	{
		if(m_isDirty)
		{
			RecomputeMatrices(true);
			m_isDirty = false;
		}

		return m_localMatrix;
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the local matrix and computes the position, rotation and scale from it
//
void Transform::SetLocalMatrix(const Matrix44& local)
{
	if(m_localMatrix == local)
	{
		return;
	}
	
	m_localMatrix = local;
	m_localTransform.SetMatrix(local);
	SetDirtyOnHierarchy();
}

//-----------------------------------------------------------------------------------------------
// Sets the world matrix and computes the local mat, position, rotation and scale from it
//
void Transform::SetWorldMatrix(const Matrix44& world)
{
	if(m_worldMatrix == world)
	{
		return;
	}

	m_worldMatrix = world;
	m_localMatrix = m_parent ? m_parent->GetWorldMatrix().GetInverse() * m_worldMatrix : m_worldMatrix; 
	m_localTransform.SetMatrix(m_localMatrix);
	SetDirtyOnHierarchy();
}

//-----------------------------------------------------------------------------------------------
// Sets the euler angles for the rotation matrix
//
void Transform::SetEulerAngles(const Vector3& angles)
{
	if(angles != m_localTransform.GetEulerAngles())
	{
		m_localTransform.SetEulerAngles(angles);
		SetDirtyOnHierarchy();
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the position
//
void Transform::SetPosition(const Vector3& newPos)
{
	if(m_localTransform.GetPosition() != newPos)
	{
		m_localTransform.SetPosition(newPos);
		SetDirtyOnHierarchy();
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the scale
//
void Transform::SetScale(const Vector3& newScale)
{
	if(m_localTransform.GetScale() != newScale)
	{
		m_localTransform.SetScale(newScale);
		SetDirtyOnHierarchy();
	}
}

//-----------------------------------------------------------------------------------------------
// Computes the transform matrix for the current rotation, position and scale
//
void Transform::RecomputeMatrices( bool isChild ) const
{
	m_localMatrix = m_localTransform.GetMatrix(); // Recomputes the matrix 

	if(isChild)
	{
		m_worldMatrix.SetIdentity();
		m_worldMatrix.Append(m_parent->GetWorldMatrix());
		m_worldMatrix.Append(m_localMatrix);
	}
	else
	{
		m_worldMatrix = m_localMatrix; // If there's no parent localMatrix = worldMatrix
	}
	
}

//-----------------------------------------------------------------------------------------------
// Translates the matrix by some offset
//
void Transform::Translate(const Vector3& offset)
{
	if(offset == Vector3::ZERO)
	{
		return;
	}

	m_localTransform.Translate(offset);
	SetDirtyOnHierarchy();
}

//-----------------------------------------------------------------------------------------------
// Rotates the matrix by some rotation from existing rotation
//
void Transform::RotateByEuler(const Vector3& rotateVec)
{
	if(rotateVec == Vector3::ZERO)
	{
		return;
	}

	m_localTransform.RotateByEuler(rotateVec);
	SetDirtyOnHierarchy();
}

//-----------------------------------------------------------------------------------------------
// Computes the matrix from the Matrix44::LookAt
//
void Transform::LookAt(const Vector3& worldPosition, const Vector3& worldUp /*= Vector3::UP */)
{
	Matrix44 look = Matrix44::LookAt(GetWorldPosition(), worldPosition, worldUp);
	look.Tx = look.Ty = look.Tz = 0.f;

	SetWorldMatrix(look);
}

//-----------------------------------------------------------------------------------------------
// Computes the local look at matrix and applies it to the transform
//
void Transform::LocalLookAt(const Vector3& localPosition, const Vector3& localUp /*= Vector3::UP */)
{
	m_localMatrix			=	m_localTransform.GetMatrix();
	Vector3 worldPos		=	m_localMatrix.TransformPosition3D(localPosition);
	Vector3 worldUp			=	m_localMatrix.TransformDirection3D(localUp);

	return LookAt(worldPos, worldUp);
}

//-----------------------------------------------------------------------------------------------
// Adds a child to the hierarchy
//
void Transform::AddChild(Transform* child)
{
	m_children.push_back(child);
	child->m_parent = this;
	child->SetDirtyOnHierarchy();
}

//-----------------------------------------------------------------------------------------------
// Sets dirty on all children
//
void Transform::SetDirtyOnHierarchy()
{
	m_isDirty = true;
	for(Transform* transform : m_children)
	{
		transform->SetDirtyOnHierarchy();
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the computed matrix from position, rotation and scale
//
Matrix44 transform_t::GetMatrix() const
{
	Matrix44 matrix; 
	matrix.Append(Matrix44::MakeTranslation3D(m_position));
	matrix.Append(Matrix44::MakeRotation3D(m_euler));
	matrix.Append(Matrix44::MakeScale3D(m_scale));
	
	return matrix;
}

//-----------------------------------------------------------------------------------------------
// Computes the position, rotation and scale from the given matrix and sets the values
//
void transform_t::SetMatrix(const Matrix44& mat)
{
	m_euler = mat.GetEulerAngles();
	m_position = Vector3(mat.Tx, mat.Ty, mat.Tz); // Assuming that the matrix was computed as TRS*point
	m_scale.x = mat.GetRight().GetLength();
	m_scale.y = mat.GetUp().GetLength();
	m_scale.z = mat.GetForward().GetLength();
}
