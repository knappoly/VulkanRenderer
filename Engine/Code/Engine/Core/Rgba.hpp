#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Vector4;

//-----------------------------------------------------------------------------------------------
class Rgba
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors
	Rgba(); // Default-constructs to opaque white (255,255,255,255)
	explicit Rgba( unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte=255 );
	explicit Rgba( float red, float green, float blue, float alpha = 1.f );
	explicit Rgba( const Vector4& color );

	//-----------------------------------------------------------------------------------------------
	// Member Variables
	float r;
	float g;
	float b;
	float a;

	//-----------------------------------------------------------------------------------------------
	// Color Constants
	static const Rgba WHITE;
	static const Rgba RED;
	static const Rgba BLUE;
	static const Rgba GREEN;
	static const Rgba BLACK;
	static const Rgba CYAN;
	static const Rgba MAJENTA;
	static const Rgba YELLOW;

	//-----------------------------------------------------------------------------------------------
	// Getters and Setters
	void GetAsFloats( float& out_normalizedRed, float& out_normalizedGreen, float& out_normalizedBlue, float& out_normalizedAlpha ) const;
	float* GetAsFloats() const;
	Vector4 GetAsVector() const;
	void SetAsBytes( unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte=255 );
	void SetAsFloats( float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha=1.0f );
	void ScaleRGB( float rgbScale ); // Scales (and clamps) RGB components, but not A
	void ScaleAlpha( float alphaScale ); // Scales (and clamps) Alpha, RGB is untouched
	void SetFromText(const char* text);

	//-----------------------------------------------------------------------------------------------
	// Operators
	bool operator==(const Rgba& compare) const;

	//-----------------------------------------------------------------------------------------------
	// Static functions
	static Rgba	FromFloats( float red, float green, float blue, float alpha = 1.f ); // Creates a Rgba from float values
	static Rgba FromBytes( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255 ); // Creates a Rgba from byte values
};

Rgba Interpolate( const Rgba& startColor, const Rgba& endColor, float fractionTowardEnd );