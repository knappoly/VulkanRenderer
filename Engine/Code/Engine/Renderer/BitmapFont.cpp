#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Core/StringTokenizer.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
BitmapFont::BitmapFont( const std::string& fontName, const SpriteSheet& glyphSheet,float baseAspect )
	: m_fontName(fontName)
	, m_spriteSheet(glyphSheet)
	, m_baseAspect(baseAspect)
{

}

//-----------------------------------------------------------------------------------------------
// Returns UV coords for the letters given as unicode
//
AABB2 BitmapFont::GetUVsForGlyph(int glyphUnicode) const
{
	AABB2 uv = m_spriteSheet.GetTexCoordsForSpriteCoords(glyphUnicode);
	return uv;
}

//-----------------------------------------------------------------------------------------------
// Returns the string width of the text to be displayed
//
float BitmapFont::GetStringWidth(const std::string& asciiText, float cellHeight, float aspectScale) const
{
	float cellWidth = cellHeight * (m_baseAspect * aspectScale);
	return cellWidth * static_cast<float>(asciiText.length());
}

//-----------------------------------------------------------------------------------------------
// Returns the height of the string
//
float BitmapFont::GetStringHeight(const std::string& asciiText, float cellHeight, float aspectScale) const
{
	StringTokenizer tokenizer(asciiText, "\n");
	tokenizer.Tokenize();
	return tokenizer.GetTokenCount() * cellHeight * aspectScale;
}

//-----------------------------------------------------------------------------------------------
// Returns the sprite sheet texture
//
const Texture* BitmapFont::GetSpriteSheetTexture() const
{
	return &m_spriteSheet.GetSpriteSheetTexture();
}
