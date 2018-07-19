#pragma once
#include <string>
#include <vector>
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/Rgba.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations

//-----------------------------------------------------------------------------------------------
class Image
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors
	Image(const Rgba& singlePixelColor, const std::string& name);
	Image(const std::string& imageFilePath, bool flipY = true);

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
					Rgba			GetTexel(int x, int y) const;
					void			SetTexel( int x, int y, const Rgba& color);
	const			Rgba*			GetTexelReference( int x, int y ) const;
					Rgba*			GetTexelReference( int x, int y );
					unsigned char*	GetTexelsAsByteArray() const;
					IntVector2		GetDimensions() const { return m_dimensions; }
					void			PopulateDataFromImage( unsigned char* imageData, int numComponents );
					std::string		GetPath() const { return m_imagePath; }
	static			void			GenerateDefaultTextures();
	static			Image*			GetDefaultTexture() { return s_defaultTexture; }

	//-----------------------------------------------------------------------------------------------
	// Members
private:
	IntVector2			m_dimensions;
	std::vector<Rgba>	m_texels;
	std::string			m_imagePath;

	//-----------------------------------------------------------------------------------------------
	// Static members
	static	Image*	s_defaultTexture;
};

