#include "Engine/Core/Rgba.hpp"
#include <string>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <string.h>
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Color constants
STATIC const Rgba Rgba::WHITE = Rgba(1.f, 1.f, 1.f);
STATIC const Rgba Rgba::RED = Rgba(1.f, 0.f, 0.f);
STATIC const Rgba Rgba::BLUE = Rgba(0.f, 0.f, 1.f);
STATIC const Rgba Rgba::GREEN = Rgba(0.f, 1.f, 0.f);
STATIC const Rgba Rgba::CYAN = Rgba(1.f, 0, 1.f);
STATIC const Rgba Rgba::MAJENTA = Rgba(1.f, 1.f, 0.f);
STATIC const Rgba Rgba::YELLOW = Rgba(1.f, 1.f, 0.f);
STATIC const Rgba Rgba::BLACK = Rgba(0.f, 0.f, 0.f);
 
//-----------------------------------------------------------------------------------------------
// Operator ==
//
bool Rgba::operator==(const Rgba& compare) const
{
	if(r == compare.r &&
		g == compare.g &&
		b == compare.b)
	{
		return true;
	}
	return false;
}



//-----------------------------------------------------------------------------------------------
// Constructor
//
Rgba::Rgba()
{
	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Rgba::Rgba(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte)
{
	r = redByte;
	g = greenByte;
	b = blueByte;
	a = alphaByte;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Rgba::Rgba(float red, float green, float blue, float alpha /*= 1.f*/)
{
	r = ClampFloatZeroToOne(red);
	g = ClampFloatZeroToOne(green);
	b = ClampFloatZeroToOne(blue);
	a = ClampFloatZeroToOne(alpha);
}

//-----------------------------------------------------------------------------------------------
// Sets the color from a vec4
//
Rgba::Rgba(const Vector4& color)
{
	r = color.x;
	g = color.y;
	b = color.z;
	a = color.w;
}

//-----------------------------------------------------------------------------------------------
// Returns color as floats through the arguments
//
void Rgba::GetAsFloats(float& out_normalizedRed, float& out_normalizedGreen, float& out_normalizedBlue, float& out_normalizedAlpha) const
{
	out_normalizedRed = r;
	out_normalizedGreen = g;
	out_normalizedBlue = b;
	out_normalizedAlpha = a;
}

//-----------------------------------------------------------------------------------------------
// Returns the color as floats through an array
//
float* Rgba::GetAsFloats() const
{
	float* data = new float[4] {r,g,b,a};

	return data;
}

//-----------------------------------------------------------------------------------------------
// Returns the color as a vector4
//
Vector4 Rgba::GetAsVector() const
{
	Vector4 color;
	
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;

	return color;
}

//-----------------------------------------------------------------------------------------------
// Set the RGBA values as bytes(0-255)
//
void Rgba::SetAsBytes(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte)
{
	r = (float) redByte / 255.f;
	g = (float) greenByte / 255.f;
	b = (float) blueByte / 255.f;
	a = (float) alphaByte / 255.f;
}

//-----------------------------------------------------------------------------------------------
// Sets RGBA values as floats(0.f-1.f)
//
void Rgba::SetAsFloats(float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha)
{
	r = normalizedRed;
	g = normalizedGreen;
	b = normalizedBlue;
	a = normalizedAlpha;
}

//-----------------------------------------------------------------------------------------------
// Scales the RGB values 
//
void Rgba::ScaleRGB(float rgbScale)
{
	float temp = r * rgbScale;
	if(temp > 1.f)
		r = 1.f;
	else
		r = temp;

	temp = g *  rgbScale;
	if(temp > 1.f)
		g = 1.f;
	else
		g = temp;

	temp = b * rgbScale;
	if(b > 1.f)
		b = 1.f;
	else
		b = temp;

}

//-----------------------------------------------------------------------------------------------
// Scales the alpha value
//
void Rgba::ScaleAlpha(float alphaScale)
{
	float temp = a * alphaScale;
	if(a> 1.f)
		a = 1.f;
	else
		a = temp;

}

//-----------------------------------------------------------------------------------------------
// Sets the value from text
//
void Rgba::SetFromText(const char* text)
{
	StringTokenizer tokenizer(text, ",");
	tokenizer.SetQuoteCheck(false);
	tokenizer.Tokenize();
	GUARANTEE_OR_DIE(tokenizer.GetTokenCount() >= 3 && tokenizer.GetTokenCount() <= 4, "Bad color data");
	Strings colorTokens = tokenizer.GetTokens();
	GUARANTEE_OR_DIE(!colorTokens[0].empty(), "Rgba value requires R");
	GUARANTEE_OR_DIE(!colorTokens[1].empty(), "Rgba value requires G");
	GUARANTEE_OR_DIE(!colorTokens[2].empty(), "Rgba value requires B");
	
	unsigned char byteR = (unsigned char) atoi(colorTokens[0].c_str());
	unsigned char byteG = (unsigned char) atoi(colorTokens[1].c_str());
	unsigned char byteB = (unsigned char) atoi(colorTokens[2].c_str());
	unsigned char byteA = 255;
	if(colorTokens.size() == 4)
	{
		a = (unsigned char) atoi(colorTokens[3].c_str());
	}

	SetAsBytes(byteR, byteG, byteB, byteA);
}

//-----------------------------------------------------------------------------------------------
// Creates a Rgba from float data
//
Rgba Rgba::FromFloats(float red, float green, float blue, float alpha /*= 1.f */)
{
	Rgba color(red, green, blue, alpha);
	return color;
}

//-----------------------------------------------------------------------------------------------
// Creates a Rgba from byte data
//
Rgba Rgba::FromBytes(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha /*= 255 */)
{
	Rgba color;
	color.SetAsBytes(red, green, blue, alpha);
	return color;
}

//-----------------------------------------------------------------------------------------------
// Interpolates the color over time
//
Rgba Interpolate(const Rgba& startColor, const Rgba& endColor, float fractionTowardEnd)
{
	Rgba temp;
	temp.r = Interpolate(startColor.r, endColor.r, fractionTowardEnd);
	temp.g = Interpolate(startColor.g, endColor.g, fractionTowardEnd);
	temp.b = Interpolate(startColor.b, endColor.b, fractionTowardEnd);
	temp.a = Interpolate(startColor.a, endColor.a, fractionTowardEnd);

	return temp;
}
