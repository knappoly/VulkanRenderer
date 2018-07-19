#include "Engine/Renderer/SpriteAnimNew/SpriteAnimNew.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/SpriteAnimNew/SpriteAnimNewDefinition.hpp"
#include "Engine/Renderer/Sprite.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteAnimNew::SpriteAnimNew(const SpriteAnimNewDefinition* animDef)
{
	m_animDef = animDef;
}

//-----------------------------------------------------------------------------------------------
// Updates the animation
//
void SpriteAnimNew::Update(float deltaSeconds)
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

	m_currentSprite = m_animDef->GetSpriteAtTime(m_elapsedSeconds);
}

//-----------------------------------------------------------------------------------------------
// Plays the animation from the start
//
void SpriteAnimNew::PlayFromStart()
{
	m_isFinished = false;
	m_elapsedSeconds = 0.f;
	m_isPlaying = true;
}

//-----------------------------------------------------------------------------------------------
// Returns the total duration of the animation
//
float SpriteAnimNew::GetDuration() const
{
	return m_animDef->GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the ratio of elapsedSeconds to total duration
//
float SpriteAnimNew::GetElapsedFraction() const
{
	return m_elapsedSeconds * 1.f / GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the remaining time to finish the animation
//
float SpriteAnimNew::GetRemainingSeconds() const
{
	return GetDuration() - m_elapsedSeconds;
}

//-----------------------------------------------------------------------------------------------
// Returns the fraction of the remaining time 
//
float SpriteAnimNew::GetRemainingFraction() const
{
	return GetRemainingSeconds() / m_animDef->GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the texture of the current Sprite
//
const	Texture* SpriteAnimNew::GetTexture() const
{
	return m_animDef->GetSpriteAtTime(m_elapsedSeconds)->GetTexture();
}

//-----------------------------------------------------------------------------------------------
// Returns the current Sprite
//
Sprite* SpriteAnimNew::GetCurrentSprite() const
{
	return m_currentSprite;
}

//-----------------------------------------------------------------------------------------------
// Returns the name of the animation
//
std::string SpriteAnimNew::GetName() const
{
	return m_animDef->m_name;
}
