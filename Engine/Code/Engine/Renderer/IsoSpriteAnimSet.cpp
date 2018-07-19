#include "Engine/Renderer/IsoSpriteAnimSet.hpp"
#include "Engine/Renderer/IsoSpriteAnim.hpp"
#include "Engine/Renderer/IsoSprite.hpp"
#include "Engine/Renderer/IsoSpriteAnimSetDefinition.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
IsoSpriteAnimSet::IsoSpriteAnimSet(IsoSpriteAnimSetDefinition& animSetDef)
{
	m_isoAnimSetDef = &animSetDef;
	std::map<std::string, IsoSpriteAnimDefinition*>::iterator found = m_isoAnimSetDef->m_namedAnimDefs.begin();
	while(found != m_isoAnimSetDef->m_namedAnimDefs.end())
	{
		IsoSpriteAnim* animClip = new IsoSpriteAnim(*found->second);
		m_namedAnims[found->first] = animClip;
		found++;
	}
	SetCurrentAnim(m_defaultAnimName);
}

//-----------------------------------------------------------------------------------------------
// Sets the current animation from the set
//
void IsoSpriteAnimSet::SetCurrentAnim(const std::string& animName)
{
	m_currentAnim = m_namedAnims[animName];
}

//-----------------------------------------------------------------------------------------------
// Returns the current texture
//
const Texture* IsoSpriteAnimSet::GetCurrentTexture() const
{
	return m_currentAnim->GetTexture();
}

//-----------------------------------------------------------------------------------------------
// Returns the current IsoSprite
//
IsoSprite* IsoSpriteAnimSet::GetCurrentIsoSprite() const
{
	return m_currentAnim->GetCurrentIsoSprite();
}

//-----------------------------------------------------------------------------------------------
// Returns the current animation name
//
std::string IsoSpriteAnimSet::GetCurrentAnimName() const
{
	return m_currentAnim->GetName();
}

//-----------------------------------------------------------------------------------------------
// Updates the animset and the current animation
//
void IsoSpriteAnimSet::Update(float deltaSeconds)
{
	m_currentAnim->Update(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------
// Starts the animation
//
void IsoSpriteAnimSet::StartAnim(const std::string& animName)
{
	std::map<std::string, IsoSpriteAnim*>::iterator found = m_namedAnims.find(animName);
	if(found == m_namedAnims.end())
		SetCurrentAnim("Idle"); //Plays idle if the animation is not found
	else
		SetCurrentAnim(animName);

	m_currentAnim->PlayFromStart();
}

//-----------------------------------------------------------------------------------------------
// Has the current animation finished
//
bool IsoSpriteAnimSet::IsCurrentAnimFinished()
{
	return m_currentAnim->IsFinished();
}
