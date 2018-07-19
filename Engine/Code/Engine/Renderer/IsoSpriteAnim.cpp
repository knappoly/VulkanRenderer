#include "Engine/Renderer/IsoSpriteAnim.hpp"
#include "Engine/Renderer/IsoSpriteAnimDefinition.hpp"
#include "Engine/Renderer/IsoSprite.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
IsoSpriteAnim::IsoSpriteAnim(const IsoSpriteAnimDefinition& animDef)
{
	m_animDef = &animDef;
}

//-----------------------------------------------------------------------------------------------
// Updates the animation
//
void IsoSpriteAnim::Update(float deltaSeconds)
{
	if(!m_isPlaying)
		deltaSeconds = 0.f;

	m_elapsedSeconds += deltaSeconds;
	
	if(m_elapsedSeconds > GetDuration())
	{
		if(m_animDef->m_isLooping)
			PlayFromStart();

		else
		{
			m_isFinished = true;
			m_isPlaying = false;
			m_elapsedSeconds -= deltaSeconds;
		}
	}

	m_currentIsoSprite = m_animDef->GetIsoSpriteAtTime(m_elapsedSeconds);
}

//-----------------------------------------------------------------------------------------------
// Resets the elapsedSeconds and plays from the start 
//
void IsoSpriteAnim::PlayFromStart()
{
	m_isFinished = false;
	m_elapsedSeconds = 0.f;
	m_isPlaying = true;
}

//-----------------------------------------------------------------------------------------------
// Returns the total duration of the animation
//
float IsoSpriteAnim::GetDuration() const
{
	return m_animDef->GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the ratio of elapsedSeconds to total duration
//
float IsoSpriteAnim::GetElapsedFraction() const
{
	return m_elapsedSeconds * 1.f / GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the remaining time to finish the animation
//
float IsoSpriteAnim::GetRemainingSeconds() const
{
	return GetDuration() - m_elapsedSeconds;
}

//-----------------------------------------------------------------------------------------------
// Returns the fraction of the remaining time 
//
float IsoSpriteAnim::GetRemainingFraction() const
{
	return GetRemainingSeconds() / m_animDef->GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the texture of the current isoSprite
//
const Texture* IsoSpriteAnim::GetTexture() const
{
	return m_animDef->GetIsoSpriteAtTime(m_elapsedSeconds)->GetTexture();
}

//-----------------------------------------------------------------------------------------------
// Returns the current isoSprite
//
IsoSprite* IsoSpriteAnim::GetCurrentIsoSprite() const
{
	return m_currentIsoSprite;
}

//-----------------------------------------------------------------------------------------------
// Returns the name of the animation
//
std::string IsoSpriteAnim::GetName() const
{
	return m_animDef->m_name;
}
