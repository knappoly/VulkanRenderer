#pragma once
#include <string>
#include "ThirdParty/TinyXML2/tinyxml2.h"
#include <vector>

typedef tinyxml2::XMLElement XMLElement;


//-----------------------------------------------------------------------------------------------
// Forward declarations
class IntRange;
class FloatRange;
class Vector2;
class IntVector2;
class Rgba;
class AABB2;
class Vector3;

//-----------------------------------------------------------------------------------------------
// XML Attribute parsers for each data structure
int							ParseXmlAttribute( const XMLElement& element, const char* attributeName, int defaultValue );
char						ParseXmlAttribute( const XMLElement& element, const char* attributeName, char defaultValue );
bool						ParseXmlAttribute( const XMLElement& element, const char* attributeName, bool defaultValue );
float						ParseXmlAttribute( const XMLElement& element, const char* attributeName, float defaultValue );
Rgba						ParseXmlAttribute( const XMLElement& element, const char* attributeName, const Rgba& defaultValue );
Vector2						ParseXmlAttribute( const XMLElement& element, const char* attributeName, const Vector2& defaultValue );
Vector3						ParseXmlAttribute( const XMLElement& element, const char* attributeName, const Vector3& defaultValue );
AABB2						ParseXmlAttribute(const XMLElement& element, const char* attributeName, const AABB2& defaultValue);
IntRange					ParseXmlAttribute( const XMLElement& element, const char* attributeName, const IntRange& defaultValue );
FloatRange					ParseXmlAttribute( const XMLElement& element, const char* attributeName, const FloatRange& defaultValue );
IntVector2					ParseXmlAttribute( const XMLElement& element, const char* attributeName, const IntVector2& defaultValue );
const char*					ParseXmlAttribute( const XMLElement& element, const char* attributeName, const char* defaultValue = nullptr );
std::string					ParseXmlAttribute( const XMLElement& element, const char* attributeName, const std::string& defaultValue );
std::vector<int>			ParseXmlAttribute( const XMLElement& element, const char* attributeName, std::vector<int> defaultValue );
std::vector<std::string>	ParseXmlAttribute( const XMLElement& element, const char* attributeName, std::vector<std::string> defaultValue );

//-----------------------------------------------------------------------------------------------
// XML Inner Text parsers for each data structure
int							ParseXmlInnerText( const XMLElement& element, int defaultValue );
char						ParseXmlInnerText( const XMLElement& element, char defaultValue );
bool						ParseXmlInnerText( const XMLElement& element, bool defaultValue );
float						ParseXmlInnerText( const XMLElement& element, float defaultValue );
Rgba						ParseXmlInnerText( const XMLElement& element, const Rgba& defaultValue );
Vector2						ParseXmlInnerText( const XMLElement& element, const Vector2& defaultValue );
Vector3						ParseXmlInnerText( const XMLElement& element, const Vector3& defaultValue );
AABB2						ParseXmlInnerText( const XMLElement& element, const AABB2& defaultValue );
IntRange					ParseXmlInnerText( const XMLElement& element, const IntRange& defaultValue );
FloatRange					ParseXmlInnerText( const XMLElement& element, const FloatRange& defaultValue );
IntVector2					ParseXmlInnerText( const XMLElement& element, const IntVector2& defaultValue );
std::string					ParseXmlInnerText( const XMLElement& element, const char* defaultValue = nullptr );
std::string					ParseXmlInnerText( const XMLElement& element, const std::string& defaultValue );
std::vector<int>			ParseXmlInnerText( const XMLElement& element, std::vector<int> defaultValue );
std::vector<std::string>	ParseXmlInnerText( const XMLElement& element, std::vector<std::string> defaultValue );