#pragma once
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;
class AABB2;
class Vector2;
class SpriteAnimSetDefinition;
class SpriteAnim;

//-----------------------------------------------------------------------------------------------
class SpriteAnimSet
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructor/Destructor
	SpriteAnimSet( SpriteAnimSetDefinition* animSetDef );
	~SpriteAnimSet();

	//-----------------------------------------------------------------------------------------------
	//	Methods
			void 			Update( float deltaSeconds );
			void 			StartAnim( const std::string& animName );
			void 			SetCurrentAnim( const std::string& animName );
	const	Texture& 		GetCurrentTexture() const;
			AABB2 			GetCurrentUVs() const;
			Vector2			GetTexCoordsAtMins() const;
			Vector2			GetTexCoordsAtMaxs() const;
			std::string		GetCurrentAnimName() const;

	//-----------------------------------------------------------------------------------------------
	// Members
protected:
	SpriteAnimSetDefinition*				m_animSetDef = nullptr;
	std::map< std::string, SpriteAnim* >	m_namedAnims;
	SpriteAnim*								m_currentAnim = nullptr;
};

