#pragma once
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Sprite;
class SpriteAnimNewDefinition;
class Texture;

//-----------------------------------------------------------------------------------------------
class SpriteAnimNew
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	SpriteAnimNew( const SpriteAnimNewDefinition* animDef );
	~SpriteAnimNew(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void		Update( float deltaSeconds );
			void		PlayFromStart();
			bool		IsFinished() const { return m_isFinished; } 
			float		GetDuration() const; 
			float		GetElapsedSeconds() const { return m_elapsedSeconds; } // Returns the amount of time that has elapsed since animation started
			float		GetElapsedFraction() const; // Returns the elapsed seconds as a fraction of duration
			float		GetRemainingSeconds() const;
			float		GetRemainingFraction() const;
	const	Texture*	GetTexture() const;
			Sprite*		GetCurrentSprite() const;
			std::string	GetName() const;

	//-----------------------------------------------------------------------------------------------
	// Members
	const	SpriteAnimNewDefinition*	m_animDef = nullptr;
			Sprite*						m_currentSprite = nullptr;
			bool						m_isPlaying = true;
			bool						m_isFinished = false;
			float						m_elapsedSeconds = 0.f;
};

