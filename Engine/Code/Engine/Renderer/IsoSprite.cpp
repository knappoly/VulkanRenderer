#include "Engine/Renderer/IsoSprite.hpp"
#include "Engine/Renderer/IsoSpriteDefinition.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
IsoSprite::IsoSprite(const IsoSpriteDefinition& definition)
	: Sprite()
	, m_isoSpriteDef(&definition)
{
	m_currentSpriteParams = m_isoSpriteDef->GetDefaultFacingSpriteParams();
	m_spriteDef = SpriteDefinition::s_definitions[m_currentSpriteParams->m_name];
}

//-----------------------------------------------------------------------------------------------
// Returns the scale for the iso sprite
//
Vector2 IsoSprite::GetScale() const
{
	return m_currentSpriteParams->m_scale;
}

//-----------------------------------------------------------------------------------------------
// Sets the definition for the isoSprite
//
void IsoSprite::SetDefinition(const IsoSpriteDefinition& definition)
{
	m_isoSpriteDef = &definition;
}

//-----------------------------------------------------------------------------------------------
// Sets the current sprite definition depending on the camera's forward
//
void IsoSprite::SetCurrentSpriteForCamera(const Vector3& camForward, const Vector3& cameraRight, const Vector3& objectForward)
{
	m_currentSpriteParams = m_isoSpriteDef->GetSpriteParamForCamera(camForward, cameraRight, objectForward);
	m_spriteDef = SpriteDefinition::GetDefinition(m_currentSpriteParams->m_name);
}
