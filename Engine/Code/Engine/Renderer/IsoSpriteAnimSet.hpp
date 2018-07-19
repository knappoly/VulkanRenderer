#pragma once
#include <string>
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class IsoSpriteAnimSetDefinition;
class IsoSprite;
class Texture;
class IsoSpriteAnim;

//-----------------------------------------------------------------------------------------------
class IsoSpriteAnimSet
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IsoSpriteAnimSet( IsoSpriteAnimSetDefinition& animSetDef );
	~IsoSpriteAnimSet(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			void 			SetCurrentAnim( const std::string& animName );
	const	Texture* 		GetCurrentTexture() const;
			IsoSprite*		GetCurrentIsoSprite() const;
			std::string		GetCurrentAnimName() const;

	//-----------------------------------------------------------------------------------------------
	// Methods
			void 			Update( float deltaSeconds );
			void 			StartAnim( const std::string& animName );
			bool			IsCurrentAnimFinished();
	
	//-----------------------------------------------------------------------------------------------
	// Members
			IsoSpriteAnimSetDefinition*				m_isoAnimSetDef = nullptr;
			std::map< std::string, IsoSpriteAnim* >	m_namedAnims;
			IsoSpriteAnim*							m_currentAnim = nullptr;
			std::string								m_defaultAnimName = "idle";
};

