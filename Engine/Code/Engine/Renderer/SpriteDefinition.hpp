#pragma once
#include "Engine\Core\XMLUtils.hpp"
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Math\AABB2.hpp"
#include "Engine\Math\Vector2.hpp"
#include <string>
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;

//-----------------------------------------------------------------------------------------------
class SpriteDefinition
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	SpriteDefinition( const XMLElement& element );
	~SpriteDefinition(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	const		Texture*			GetTexture() const;
				AABB2				GetUV() const { return m_uvs; }
				Vector2				GetPivot() const { return m_pivot; }
				IntVector2			GetDimensions() const { return m_dimensions; }
				Vector2				GetWorldDimensions() const { return m_worldDimensions; }
				AABB2				GetSpriteLocalBounds() const { return m_spriteBounds; }
	static		SpriteDefinition*	GetDefinition( const std::string& name );

	//-----------------------------------------------------------------------------------------------
	// Methods
				void				SetDimensionsFromUVs( const AABB2& uvs );
				void				SetUVsFromPixels( const AABB2& pixels );
				void				SetLocalBoundsFromDimensions( const Vector2& dimensions, const Vector2& pivot);
	static		void				InitializeDefinitions( const std::string& filePath );

	//-----------------------------------------------------------------------------------------------
	// Members
	static		std::map<std::string, SpriteDefinition*>	s_definitions;
	const		Texture*									m_texture = nullptr;
				AABB2										m_uvs;
				IntVector2									m_dimensions;
				Vector2										m_pivot = Vector2(0.5f, 0.5f);
				std::string									m_name = "";
				int											m_ppu = 16;
				Vector2										m_worldDimensions;
				AABB2										m_spriteBounds;
};

