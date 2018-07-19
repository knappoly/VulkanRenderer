#pragma once
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward declarations
class AABB2;
class SpriteSheet;
class Texture;

//-----------------------------------------------------------------------------------------------
class BitmapFont
{
	friend class Renderer;

	//-----------------------------------------------------------------------------------------------
	// Constructor/Destructor
private:
	explicit BitmapFont( const std::string& fontName, const SpriteSheet& glyphSheet,float baseAspect ); // Can only be constructed by Renderer

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
public:
			AABB2		GetUVsForGlyph( int glyphUnicode ) const; // pass ‘A’ or 65 for A, etc.
			float		GetGlyphAspect( int glyphUnicode ) const { return m_baseAspect; (void)(glyphUnicode); } // will change later
			float		GetStringWidth( const std::string& asciiText, float cellHeight, float aspectScale ) const;
			float		GetStringHeight( const std::string& asciiText, float cellHeight, float aspectScale ) const;
	const	Texture*	GetSpriteSheetTexture() const;

	//-----------------------------------------------------------------------------------------------
	// Members
private:
	const	SpriteSheet&	m_spriteSheet; // used internally; assumed to be a 16x16 glyph sheet
			float			m_baseAspect = 1.0f; // used as the base aspect ratio for all glyphs
			std::string		m_fontName;

};