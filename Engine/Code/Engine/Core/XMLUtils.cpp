#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "../Math/AABB2.hpp"
#include "../Math/Vector3.hpp"

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for IntVector2 data
//
IntVector2 ParseXmlAttribute(const XMLElement& element, const char* attributeName, const IntVector2& defaultValue)
{
	IntVector2 vec2 = defaultValue;
	const char* vecText = element.Attribute(attributeName);

	if(vecText)
		vec2.SetFromText(vecText);

	return vec2;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for string data
//
const char* ParseXmlAttribute(const XMLElement& element, const char* attributeName, const char* defaultValue)
{
	const char* text = defaultValue;
	const char* textValue = element.Attribute(attributeName);

	if(textValue)
		text = textValue;

	return text;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for int data
//
int ParseXmlAttribute(const XMLElement& element, const char* attributeName, int defaultValue)
{
	int value = defaultValue;
	const char* valueText = element.Attribute(attributeName);

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for char data
//
char ParseXmlAttribute(const XMLElement& element, const char* attributeName, char defaultValue)
{
	char value = defaultValue;

	const char* valueText = element.Attribute(attributeName);

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for boolean data
//
bool ParseXmlAttribute(const XMLElement& element, const char* attributeName, bool defaultValue)
{
	bool value = defaultValue;
	const char* valueText = element.Attribute(attributeName);

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for float data
//
float ParseXmlAttribute(const XMLElement& element, const char* attributeName, float defaultValue)
{
	float value = defaultValue;
	
	const char* valueText = element.Attribute(attributeName);

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for RGBA data
//
Rgba ParseXmlAttribute(const XMLElement& element, const char* attributeName, const Rgba& defaultValue)
{
	Rgba color = defaultValue;
	const char* colorText = element.Attribute(attributeName);

	if(colorText)
		color.SetFromText(colorText);

	return color;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for Vector2 data
//
Vector2 ParseXmlAttribute(const XMLElement& element, const char* attributeName, const Vector2& defaultValue)
{
	Vector2 vec2 = defaultValue;
	const char* vecText = element.Attribute(attributeName);

	if(vecText)
		vec2.SetFromText(vecText);

	return vec2;
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for IntRange data
//
IntRange ParseXmlAttribute(const XMLElement& element, const char* attributeName, const IntRange& defaultValue)
{
	IntRange range = defaultValue;
	const char* rangeText = element.Attribute(attributeName);

	if(rangeText)
		range.SetFromText(rangeText);

	return range;
	
}

//-----------------------------------------------------------------------------------------------
// Parses the XML attribute for FloatRange data
//
FloatRange ParseXmlAttribute(const XMLElement& element, const char* attributeName, const FloatRange& defaultValue)
{
	FloatRange range = defaultValue;
	const char* rangeText = element.Attribute(attributeName);

	if(rangeText)
		range.SetFromText(rangeText);

	return range;
}

//-----------------------------------------------------------------------------------------------
// Parses the xml attribute for a list of strings
//
std::vector<std::string> ParseXmlAttribute(const XMLElement& element, const char* attributeName, const std::vector<std::string> defaultValue)
{
	std::vector<std::string> strings = defaultValue;
	const char* stringsText = element.Attribute(attributeName);

	if(stringsText)
	{
		StringTokenizer tokenizer(stringsText, ",");
		tokenizer.Tokenize();
		strings = tokenizer.GetTokens();
	}

	return strings;
}

//-----------------------------------------------------------------------------------------------
// Parses the xml attribute for a list of ints
//
std::vector<int> ParseXmlAttribute(const XMLElement& element, const char* attributeName, const std::vector<int> defaultValue)
{
	std::vector<int> ints = defaultValue;
	const char* intsText = element.Attribute(attributeName);

	if(intsText)
	{
		StringTokenizer tokenizer(intsText, ",");
		tokenizer.Tokenize();
		std::vector<std::string> tokens = tokenizer.GetTokens(); 
		for(size_t index = 0; index < tokens.size(); ++index)
		{
			int value;
			SetFromText(tokens[index].c_str(), value);
			ints.push_back(value);
		}
	}

	return ints;
}

//-----------------------------------------------------------------------------------------------
//  Parses the xml attribute for a AABB2 
//
AABB2 ParseXmlAttribute(const XMLElement& element, const char* attributeName, const AABB2& defaultValue)
{
	AABB2 value = defaultValue;
	const char* aabbText = element.Attribute(attributeName);

	if(aabbText)
		value.SetFromText(aabbText);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the xml attribute for a std::string
//
std::string ParseXmlAttribute(const XMLElement& element, const char* attributeName, const std::string& defaultValue)
{
	std::string text = defaultValue;
	const char* textValue = element.Attribute(attributeName);

	if(textValue)
		text = textValue;

	return text;
}

//-----------------------------------------------------------------------------------------------
// Parses the xml attribute for a Vector3
//
Vector3 ParseXmlAttribute(const XMLElement& element, const char* attributeName, const Vector3& defaultValue)
{
	Vector3 vec3 = defaultValue;
	const char* vecText = element.Attribute(attributeName);

	if(vecText)
		vec3.SetFromText(vecText);

	return vec3;
}

//-----------------------------------------------------------------------------------------------
// Parses the xml inner text for an integer
//
int ParseXmlInnerText(const XMLElement& element, int defaultValue)
{
	int value = defaultValue;
	const char* valueText = element.GetText();

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a char
//
char ParseXmlInnerText(const XMLElement& element, char defaultValue)
{
	char value = defaultValue;

	const char* valueText = element.GetText();

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a bool
//
bool ParseXmlInnerText(const XMLElement& element, bool defaultValue)
{
	bool value = defaultValue;
	const char* valueText = element.GetText();

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a float
//
float ParseXmlInnerText(const XMLElement& element, float defaultValue)
{
	float value = defaultValue;

	const char* valueText = element.GetText();

	if(valueText)
		SetFromText(valueText, value);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a RGBA
//
Rgba ParseXmlInnerText(const XMLElement& element, const Rgba& defaultValue)
{
	Rgba color = defaultValue;
	const char* colorText = element.GetText();

	if(colorText)
		color.SetFromText(colorText);

	return color;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a Vector2
//
Vector2 ParseXmlInnerText(const XMLElement& element, const Vector2& defaultValue)
{
	Vector2 vec2 = defaultValue;
	const char* vecText = element.GetText();
	if(vecText)
		vec2.SetFromText(vecText);

	return vec2;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a AABB2
//
AABB2 ParseXmlInnerText(const XMLElement& element, const AABB2& defaultValue)
{
	AABB2 value = defaultValue;
	const char* aabbText = element.GetText();

	if(aabbText)
		value.SetFromText(aabbText);

	return value;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a IntRange
//
IntRange ParseXmlInnerText(const XMLElement& element, const IntRange& defaultValue)
{
	IntRange range = defaultValue;
	const char* rangeText = element.GetText();

	if(rangeText)
		range.SetFromText(rangeText);

	return range;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a FloatRange
//
FloatRange ParseXmlInnerText(const XMLElement& element, const FloatRange& defaultValue)
{
	FloatRange range = defaultValue;
	const char* rangeText = element.GetText();

	if(rangeText)
		range.SetFromText(rangeText);

	return range;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for an IntVector2
//
IntVector2 ParseXmlInnerText(const XMLElement& element, const IntVector2& defaultValue)
{
	IntVector2 vec2 = defaultValue;
	const char* vecText = element.GetText();
	if(vecText)
		vec2.SetFromText(vecText);

	return vec2;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a string
//
std::string ParseXmlInnerText(const XMLElement& element, const char* defaultValue /*= nullptr */)
{
	std::string text = defaultValue;
	const char* textValue = element.GetText();

	if(textValue)
		text = textValue;

	return text;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a string
//
std::string ParseXmlInnerText(const XMLElement& element, const std::string& defaultValue)
{
	std::string text = defaultValue;
	const char* textValue = element.GetText();

	if(textValue)
		text = textValue;

	return text;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a std::vector of Ints
//
std::vector<int> ParseXmlInnerText(const XMLElement& element, std::vector<int> defaultValue)
{
	std::vector<int> ints = defaultValue;
	const char* intsText = element.GetText();

	if(intsText)
	{
		StringTokenizer tokenizer(intsText, ",");
		tokenizer.Tokenize();
		std::vector<std::string> tokens = tokenizer.GetTokens(); 
		for(size_t index = 0; index < tokens.size(); ++index)
		{
			int value;
			SetFromText(tokens[index].c_str(), value);
			ints.push_back(value);
		}
	}

	return ints;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text for a std::vector of strings
//
std::vector<std::string> ParseXmlInnerText(const XMLElement& element, std::vector<std::string> defaultValue)
{
	std::vector<std::string> strings = defaultValue;
	const char* stringsText = element.GetText();
	if(stringsText)
	{
		StringTokenizer tokenizer(stringsText, ",");
		tokenizer.Tokenize();
		strings = tokenizer.GetTokens();
	}

	return strings;
}

//-----------------------------------------------------------------------------------------------
// Parses the inner text of the element for a Vector3
//
Vector3 ParseXmlInnerText(const XMLElement& element, const Vector3& defaultValue)
{
	Vector3 vec3 = defaultValue;
	const char* vecText = element.GetText();

	if(vecText)
		vec3.SetFromText(vecText);

	return vec3;
}
