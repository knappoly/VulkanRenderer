#include "Engine/Renderer/SpriteAnimation.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteAnimation::SpriteAnimation( const SpriteSheet& spriteSheet, float durationSeconds, SpriteAnimMode playbackMode, int startSpriteIndex, int endSpriteIndex )
{
	m_spriteSheet = new SpriteSheet(spriteSheet.GetSpriteSheetTexture(), 5, 5);
	m_spriteAnimMode = playbackMode;
	m_durationSeconds = durationSeconds;
	m_startSpriteIndex = startSpriteIndex;
	m_endSpriteIndex = endSpriteIndex;
	m_timePerFrame = durationSeconds / GetNumSprites();
	m_isPlaying = true;
	m_currentIndex = m_startSpriteIndex;
}

//-----------------------------------------------------------------------------------------------
// Updates the animation
//
void SpriteAnimation::Update(float deltaSeconds)
{
	if(!IsPlaying())
		deltaSeconds = 0.f;
	m_elapsedSeconds += deltaSeconds;
	m_fractionElapsed = m_elapsedSeconds * 1.f/m_durationSeconds;
	if(m_elapsedSeconds >= m_durationSeconds)
	{
		switch (m_spriteAnimMode)
		{
		case SpriteAnimMode::SPRITE_ANIM_MODE_LOOPING:
			Reset();
			break;
		case SpriteAnimMode::SPRITE_ANIM_MODE_PLAY_TO_END:
			m_isFinished = true;
			m_isPlaying = false;
			break;

		}
	}
	else
	{
		
		m_currentIndex = m_startSpriteIndex + static_cast<int>(m_fractionElapsed * GetNumSprites());
		
	}


}

//-----------------------------------------------------------------------------------------------
// Returns the current texture coordinates
//
AABB2 SpriteAnimation::GetCurrentTexCoords() const
{
	return m_spriteSheet->GetTexCoordsForSpriteCoords(m_currentIndex);
}

//-----------------------------------------------------------------------------------------------
// Returns the texture used by the sprite sheet
//
const Texture* SpriteAnimation::GetTexture() const 
{
	const Texture* texture = &m_spriteSheet->GetSpriteSheetTexture();
	return texture;
}

//-----------------------------------------------------------------------------------------------
// Pauses the animation
//
void SpriteAnimation::Pause()
{
	m_isPlaying = false;
}

//-----------------------------------------------------------------------------------------------
// Resumes the animation
//
void SpriteAnimation::Resume()
{
	m_isPlaying = true;
}

//-----------------------------------------------------------------------------------------------
// Resets the animation and plays
//
void SpriteAnimation::Reset()
{
	m_elapsedSeconds = 0.f;
}

//-----------------------------------------------------------------------------------------------
// Returns the remaining seconds
//
float SpriteAnimation::GetSecondsRemaining() const
{
	return m_durationSeconds - m_elapsedSeconds;
}

//-----------------------------------------------------------------------------------------------
// Returns the fraction of time remaining
//
float SpriteAnimation::GetFractionRemaining() const
{
	return (1 - m_fractionElapsed);
}

// Jump to specific time
void SpriteAnimation::SetSecondsElapsed(float secondsElapsed)
{
	m_elapsedSeconds = secondsElapsed;
}

//-----------------------------------------------------------------------------------------------
// Sets the fraction of the time elapsed
// 
void SpriteAnimation::SetFractionElapsed(float fractionElapsed)
{
	m_fractionElapsed = fractionElapsed;
}

