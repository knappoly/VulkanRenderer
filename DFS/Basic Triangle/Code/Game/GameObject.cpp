#include "Game/GameObject.hpp"
#include "Engine/Renderer/Renderable.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
GameObject::GameObject()
{
	m_transform = new Transform();
	m_renderable = new Renderable();
	m_renderable->SetWatchTransform(m_transform);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
GameObject::~GameObject()
{
	delete m_renderable;
	m_renderable = nullptr;

	delete m_transform;
	m_transform = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Sets the position
//
void GameObject::SetPosition(const Vector3& pos)
{
	m_transform->SetPosition(pos);
}

//-----------------------------------------------------------------------------------------------
// Sets the euler rotation
//
void GameObject::SetEuler(const Vector3& rotation)
{
	m_transform->SetEulerAngles(rotation);
}

//-----------------------------------------------------------------------------------------------
// Sets the scale
//
void GameObject::SetScale(const Vector3& scale)
{
	m_transform->SetScale(scale);
}

//-----------------------------------------------------------------------------------------------
// Sets the renderable and the watch transform on the renderable
//
void GameObject::SetRenderable(Renderable* render)
{
	m_renderable = render;
	m_renderable->SetWatchTransform(m_transform);
}

//-----------------------------------------------------------------------------------------------
// Sets the mesh on the renderable
//
void GameObject::SetMesh(Mesh* mesh)
{
	m_renderable->SetMesh(mesh);
}

//-----------------------------------------------------------------------------------------------
// Sets the material on the renderable
//
void GameObject::SetMaterial(const Material& material)
{
	m_renderable->SetMaterial(material);
}

