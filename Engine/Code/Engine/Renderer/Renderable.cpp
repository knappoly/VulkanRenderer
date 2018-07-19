#include "Engine/Renderer/Renderable.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Material.hpp"
#include "Engine/Math/Transform.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
Renderable::Renderable()
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Renderable::~Renderable()
{
	if(m_materialInstance)
	{
		delete m_materialInstance;
		m_materialInstance = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the position
//
Vector3 Renderable::GetPosition() const
{
	return GetModelMatrix().GetTranslation();
}

//-----------------------------------------------------------------------------------------------
// Returns the pointer to a material instance
//
Material* Renderable::GetEditableMaterial()
{
	if(!m_materialInstance)
	{
		m_materialInstance = m_material->Clone();
	}

	return m_materialInstance;
}

//-----------------------------------------------------------------------------------------------
// Returns the pointer to the shared material 
//
const Material* Renderable::GetSharedMaterial() const
{
	return m_material;
}

//-----------------------------------------------------------------------------------------------
// Returns the material that's currently being used (instance takes priority)
//
const Material* Renderable::GetMaterial() const
{
	if(m_materialInstance)
	{
		return m_materialInstance;
	}
	
	return m_material;
}

//-----------------------------------------------------------------------------------------------
// Computes model matrix and returns it
//
Matrix44 Renderable::GetModelMatrix() const
{
	if(m_watchTransform == nullptr)
	{
		return Matrix44::IDENTITY;
	}

	return m_watchTransform->GetWorldMatrix();
}

//-----------------------------------------------------------------------------------------------
// Sets the model matrix on the renderable
//
void Renderable::SetModelMatrix(const Matrix44& model)
{
	m_modelMatrix = model;
}

//-----------------------------------------------------------------------------------------------
// Sets the material and invalidates the current material instance
//
void Renderable::SetMaterial(const Material& material)
{
	m_material = &material; 
	
	if(m_materialInstance)
	{
		delete m_materialInstance;
		m_materialInstance = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns true if it uses light
//
bool Renderable::IsLit() const
{
	return GetMaterial()->IsLit();
}

//-----------------------------------------------------------------------------------------------
// Returns true if the object is opaque
//
bool Renderable::IsOpaque() const
{
	return GetMaterial()->IsOpaque();
}
