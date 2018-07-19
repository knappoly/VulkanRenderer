#pragma once
#include <vector>
#include "Engine\Math\Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Mesh;
class Material;
class Light;
class Transform;

//-----------------------------------------------------------------------------------------------
class DrawCall
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	DrawCall(){}
	~DrawCall(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			int					GetSortOrder() const;
			int					GetRenderQueue() const;			
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	
	//-----------------------------------------------------------------------------------------------
	// Members
			Mesh*				m_mesh;
	const	Material*			m_material;
			Matrix44			m_model;
			std::vector<Light*> m_lights;
	const	Transform*			m_transform;
};


