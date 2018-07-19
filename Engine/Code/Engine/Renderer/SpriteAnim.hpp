#pragma once
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declaration
class SpriteAnimDefinition;
class AABB2;
class Texture;

//-----------------------------------------------------------------------------------------------
class SpriteAnim
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructor/Destructor
	SpriteAnim( const SpriteAnimDefinition* animDef );
	~SpriteAnim();

	//-----------------------------------------------------------------------------------------------
	// Methods
			void		Update( float deltaSeconds );
			void		PlayFromStart();
			bool		IsFinished() const { return m_isFinished; } 
			float		GetElapsedSeconds() const { return m_elapsedSeconds; } // Returns the amount of time that has elapsed since animation started
			float		GetElapsedFraction() const; // Returns the elapsed seconds as a fraction of duration
			float		GetRemainingSeconds() const;
			float		GetRemainingFraction() const;
	const	Texture&	GetTexture() const;
			AABB2		GetCurrentUVs() const;
			std::string GetName() const;

	//-----------------------------------------------------------------------------------------------
	// Members
protected:
	const	SpriteAnimDefinition*	m_animDef = nullptr;
			bool					m_isPlaying = true;
			bool					m_isFinished = false;
			float					m_elapsedSeconds = 0.f;
};
