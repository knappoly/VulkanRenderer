#pragma once
#include <map>
#include <string>
#include "ThirdParty/TinyXML2/tinyxml2.h"

typedef tinyxml2::XMLElement XMLElement;

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Rgba;
class Vector2;
class IntVector2;
class FloatRange;
class IntRange;

//-----------------------------------------------------------------------------------------------
class Blackboard
{
public:

	//-----------------------------------------------------------------------------------------------
	// Constructor/Destructor
	Blackboard();
	~Blackboard(){}

	//-----------------------------------------------------------------------------------------------
	// Methods
	void			PopulateFromXmlElementAttributes( const XMLElement& element );
	void			SetValue( const std::string& keyName, const std::string& newValue );

	//-----------------------------------------------------------------------------------------------
	// Accessors
	bool			GetValue( const std::string& keyName, bool defaultValue ) const;
	int				GetValue( const std::string& keyName, int defaultValue ) const;
	float			GetValue( const std::string& keyName, float defaultValue ) const;
	std::string		GetValue( const std::string& keyName, std::string defaultValue ) const;
	std::string		GetValue( const std::string& keyName, const char* defaultValue ) const;
	Rgba			GetValue( const std::string& keyName, const Rgba& defaultValue ) const;
	Vector2			GetValue( const std::string& keyName, const Vector2& defaultValue ) const;
	IntVector2		GetValue( const std::string& keyName, const IntVector2& defaultValue ) const;
	FloatRange		GetValue( const std::string& keyName, const FloatRange& defaultValue ) const;
	IntRange		GetValue( const std::string& keyName, const IntRange& defaultValue ) const;


	//-----------------------------------------------------------------------------------------------
	// Members

private:
	std::map< std::string, std::string > m_keyValuePairs; // Registry of all values present in the xml file
};