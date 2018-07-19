#pragma once
#include "Engine/Math/Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Mesh;
class Material;
class Transform;

//-----------------------------------------------------------------------------------------------
class Renderable
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Renderable();
	~Renderable();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			Mesh*		GetMesh() const { return m_mesh; }
			Vector3		GetPosition() const;
			Material*	GetEditableMaterial();
	const	Material*	GetSharedMaterial() const;
	const	Material*	GetMaterial() const;
			Matrix44	GetModelMatrix() const;
			void		SetModelMatrix( const Matrix44& model );
			void		SetMaterial( const Material& material );
			void		SetMesh( Mesh* mesh ) { m_mesh = mesh; }
			void		SetWatchTransform( const Transform* transform ) { m_watchTransform = transform; }
			bool		IsLit() const;
			bool		IsOpaque() const;
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	
	//-----------------------------------------------------------------------------------------------
	// Members
			Mesh*		m_mesh;
			int			m_sortOrder = 0;
	const	Material*	m_material;
			Material*	m_materialInstance = nullptr;
			Matrix44	m_modelMatrix;
	const	Transform*	m_watchTransform;
};


