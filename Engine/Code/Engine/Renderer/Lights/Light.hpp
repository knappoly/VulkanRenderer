#pragma once
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Structures/LightStructure.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Transform;

//-----------------------------------------------------------------------------------------------
class Light
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Light();
	virtual ~Light();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			bool	IsShadowCasting() const { return m_isShadowCasting; }
			void	SetColor( const Rgba& color ) { m_lightDesc.color = color.GetAsVector(); }
			Rgba	GetColor() const;
			void	SetIntensity( float intensity );
			float	GetIntensity() const { return m_lightDesc.color.a; }
			void	SetPosition( const Vector3& position );
			Vector3	GetLocalPosition() const;
			Vector3 GetWorldPosition() const;
			Vector3	GetDirection() const;
			void	SetDirection( const Vector3& dir ) { m_lightDesc.direction = dir; }
			void	SetAttenuation( const Vector3& atten ) { m_lightDesc.attenuation = atten; }
			Vector3 GetAttenuation() const { return m_lightDesc.attenuation; }
			float	GetDirFactor() const { return m_lightDesc.directionFactor; }
			float	GetSpotFactor() const { return m_lightDesc.spotFactor; }
			float	GetInnerDot() const { return m_lightDesc.dotInnerAngle; }
			float	GetOuterDot() const { return m_lightDesc.dotOuterAngle; }
			void	SetViewProjection( const Matrix44& vp );

	//-----------------------------------------------------------------------------------------------
	// Methods
			void	SetupPointLight( const Vector3& position, const Rgba& color = Rgba::WHITE, float intensity = 1.f, const Vector3& attenuation = Vector3(1.f, 0.f, 0.f) );
			void	SetupDirectionalLight( const Vector3& position, const Vector3& dir, const Rgba& color = Rgba::WHITE, float intensity = 1.f );
			void	SetupSpotlight( const Vector3& position, const Vector3& dir, float innerAngle, float outerAngle, const Rgba& color = Rgba::WHITE, float intensity = 1.f );
			void	PreRender();
			void	DebugRender() const;

	//-----------------------------------------------------------------------------------------------
	// Members
	bool			m_isShadowCasting = false;
	Transform*		m_transform;
	LightStructure	m_lightDesc;
};

