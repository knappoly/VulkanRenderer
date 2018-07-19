#include "Engine/Renderer/SpriteAnim.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
// 
SpriteAnim::SpriteAnim( const SpriteAnimDefinition* animDef ): m_animDef(animDef)
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
SpriteAnim::~SpriteAnim()
{
	m_animDef = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Updates the animation 
//
void SpriteAnim::Update( float deltaSeconds )
{
	if(!m_isPlaying)
		deltaSeconds = 0.f;
	
	m_elapsedSeconds += deltaSeconds;
	//m_fractionElapsed = m_elapsedSeconds * 1.f/m_durationSeconds;
	if(GetElapsedFraction() >= 1.f)
	{
		if(m_animDef->m_isLooping)
			PlayFromStart();

		else
		{
			m_isFinished = true;
			m_isPlaying = false;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the current UVs of the animation
//
AABB2 SpriteAnim::GetCurrentUVs() const
{
	int curSpriteIndex = m_animDef->GetSpriteIndexAtTime(m_elapsedSeconds);
	AABB2 UVs = m_animDef->m_spriteSheet->GetTexCoordsForSpriteCoords(curSpriteIndex);

	return UVs;
}

//-----------------------------------------------------------------------------------------------
// Returns to start of animation and starts playing
//
void SpriteAnim::PlayFromStart()
{
	m_elapsedSeconds = 0.f;
	m_isPlaying = true;
}

//-----------------------------------------------------------------------------------------------
// Returns the texture used by the sprite animation
//
const Texture& SpriteAnim::GetTexture() const 
{
	return m_animDef->m_spriteSheet->GetSpriteSheetTexture();
}

//-----------------------------------------------------------------------------------------------
// Returns the elapsed seconds as a fraction of the total duration
//
float SpriteAnim::GetElapsedFraction() const
{
	return m_elapsedSeconds * 1.f / m_animDef->GetDuration(); 
}

//-----------------------------------------------------------------------------------------------
// Returns the remaining seconds 
//
float SpriteAnim::GetRemainingSeconds() const
{
	return (m_animDef->GetDuration() - m_elapsedSeconds);
}

//-----------------------------------------------------------------------------------------------
// Returns the remaining time as a fraction of the total duration
//
float SpriteAnim::GetRemainingFraction() const
{
	return GetRemainingSeconds() * 1.f / m_animDef->GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the name of the animation clip
//
std::string SpriteAnim::GetName() const
{
	return m_animDef->m_name;
}