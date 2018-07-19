#include "Engine/Renderer/Lights/Light.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/Renderer/DebugRenderUtils.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes

//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
Light::Light()
{
	m_transform = new Transform();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Light::~Light()
{
	delete m_transform;
	m_transform = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Returns the color
//
Rgba Light::GetColor() const
{
	return Rgba(m_lightDesc.color);
}

//-----------------------------------------------------------------------------------------------
// Sets the intensity as the alpha value of the color
//
void Light::SetIntensity(float intensity)
{
	m_lightDesc.color.a = intensity;
}

//-----------------------------------------------------------------------------------------------
// Sets the position of the light
//
void Light::SetPosition(const Vector3& position)
{
	m_transform->SetPosition(position);
}

//-----------------------------------------------------------------------------------------------
// Returns the position of the light
//
Vector3 Light::GetLocalPosition() const
{
	return m_transform->GetLocalPosition();
}

//-----------------------------------------------------------------------------------------------
// Returns the world position of the light
//
Vector3 Light::GetWorldPosition() const
{
	return m_transform->GetWorldMatrix().GetTranslation();
}

//-----------------------------------------------------------------------------------------------
// Returns the forward as the direction of the light
//
Vector3 Light::GetDirection() const
{
	return m_transform->GetWorldMatrix().GetForward();
}

//-----------------------------------------------------------------------------------------------
// Sets the view projection matrix for shadow mapping 
//
void Light::SetViewProjection(const Matrix44& vp)
{
	m_lightDesc.shadowVP = vp;
}

//-----------------------------------------------------------------------------------------------
// Sets up the light as a point light
//
void Light::SetupPointLight(const Vector3& position, const Rgba& color /*= Rgba::WHITE*/, float intensity /*= 1.f*/, const Vector3& attenuation /*= Vector3(1.f, 0.f, 0.f) */)
{
	SetPosition(position);
	m_lightDesc.attenuation = attenuation;
	m_lightDesc.color = color.GetAsVector();
	m_lightDesc.color.a = intensity;
	m_lightDesc.directionFactor = 0.f;
	m_lightDesc.spotFactor = 0.f;
}

//-----------------------------------------------------------------------------------------------
// Sets up the light as a directional light
//
void Light::SetupDirectionalLight(const Vector3& position, const Vector3& angles, const Rgba& color /*= Rgba::WHITE*/, float intensity /*= 1.f */)
{
	SetPosition(position);
	m_lightDesc.color = color.GetAsVector();
	m_lightDesc.color.a = intensity;
	
	//Vector3 euler = CartesianToSpherical(dir);
	m_transform->RotateByEuler(angles);
	m_lightDesc.direction = m_transform->GetForward();
	m_lightDesc.directionFactor = 1.f;
	m_lightDesc.spotFactor = 0.f;
	m_lightDesc.dotInnerAngle = 1.f;
	m_lightDesc.dotOuterAngle = 1.f;
	m_lightDesc.attenuation = Vector3(1.f, 0.f, 0.f);
	m_isShadowCasting = true;
	m_lightDesc.isShadowCasting = 1.f;
}

//-----------------------------------------------------------------------------------------------
// Sets up the light as a spot light
//
void Light::SetupSpotlight(const Vector3& position, const Vector3& dir, float innerAngle, float outerAngle, const Rgba& color /*= Rgba::WHITE*/, float intensity /*= 1.f */)
{
	SetPosition(position);
	m_lightDesc.color = color.GetAsVector();
	m_lightDesc.color.a = intensity;
	m_lightDesc.direction = dir;

	Vector3 euler = CartesianToSpherical(dir);
	m_transform->RotateByEuler(euler.z, 90.f - euler.y, 0.f);
	m_lightDesc.directionFactor = 1.f;
	m_lightDesc.spotFactor = 1.f;

	m_lightDesc.dotInnerAngle = CosDegrees(innerAngle);
	m_lightDesc.dotOuterAngle = CosDegrees(outerAngle);
	m_lightDesc.attenuation = Vector3(0.f, 1.f, 0.0f);
}

//-----------------------------------------------------------------------------------------------
// Sets the matrices proper and makes data available for render
//
void Light::PreRender()
{
	m_lightDesc.direction = m_transform->GetWorldMatrix().GetForward();
	if(IsShadowCasting())
	{
		m_lightDesc.isShadowCasting = 1.f;
	}
}

//-----------------------------------------------------------------------------------------------
// Debug renders the light
//
void Light::DebugRender() const
{
	//DebugRenderPoint(0.f, GetWorldPosition(), Rgba(m_lightDesc.color), Rgba(m_lightDesc.color));
	DebugRenderWireSphere(0.f, GetWorldPosition(), 0.5f, Rgba(m_lightDesc.color));
	DebugRenderBasis(0.f, m_transform->GetWorldMatrix());
}
