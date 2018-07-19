#pragma once
#include "Engine/Math/Transform.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Renderable;
class Mesh;
class Material;

//-----------------------------------------------------------------------------------------------
class GameObject
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	GameObject();
	virtual ~GameObject();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void			SetPosition( const Vector3& pos );
	void			SetEuler( const Vector3& rotation );
	void			SetScale( const Vector3& scale );
	Renderable*		GetRenderable() const { return m_renderable; }
	void			SetRenderable( Renderable* render );
	void			SetMesh( Mesh* mesh );
	void			SetMaterial( const Material& material );
	virtual void	Update( float deltaSeconds ) { (void)(deltaSeconds); }
	virtual void	Render() const {}
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	
	//-----------------------------------------------------------------------------------------------
	// Members
	Transform*	m_transform;
	Renderable*	m_renderable;
};

