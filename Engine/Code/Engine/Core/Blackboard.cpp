#include "Engine/Core/Blackboard.hpp"
#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/Rgba.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
Blackboard::Blackboard()
{

}

//-----------------------------------------------------------------------------------------------
// Populate local game config vector with data from xml
//
void Blackboard::PopulateFromXmlElementAttributes(const XMLElement& element)
{
	std::map<std::string, std::string>::iterator found;
	for(const tinyxml2::XMLAttribute* attrib = element.FirstAttribute(); attrib; attrib = attrib->Next())
	{
		found = m_keyValuePairs.find(attrib->Name());
		if(found == m_keyValuePairs.end())
			m_keyValuePairs[attrib->Name()] = attrib->Value();
		else
			GUARANTEE_OR_DIE(false, "Duplicate attribute in the xml file");
	}
}

//-----------------------------------------------------------------------------------------------
// Change the value of a key-value pair 
//
void Blackboard::SetValue( const std::string& keyName, const std::string& newValue )
{
	std::map<std::string, std::string>::iterator found = m_keyValuePairs.find(keyName);
	if(found == m_keyValuePairs.end())
		GUARANTEE_OR_DIE(false, "Key not found in the game_config blackboard" + keyName);
	
	m_keyValuePairs[keyName] = newValue;
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
bool Blackboard::GetValue(const std::string& keyName, bool defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		bool value;
		SetFromText(found->second.c_str(), value);
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
int Blackboard::GetValue( const std::string& keyName, int defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		int value;
		SetFromText(found->second.c_str(), value);
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
float Blackboard::GetValue( const std::string& keyName, float defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		float value;
		SetFromText(found->second.c_str(), value);
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
std::string Blackboard::GetValue( const std::string& keyName, std::string defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		return found->second;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
std::string Blackboard::GetValue( const std::string& keyName, const char* defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		return found->second;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
Rgba Blackboard::GetValue( const std::string& keyName, const Rgba& defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		Rgba value;
		value.SetFromText(found->second.c_str());
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
//
Vector2 Blackboard::GetValue( const std::string& keyName, const Vector2& defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		Vector2 value;
		value.SetFromText(found->second.c_str());
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of the key
// 
IntVector2 Blackboard::GetValue( const std::string& keyName, const IntVector2& defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		IntVector2 value;
		value.SetFromText(found->second.c_str());
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of they key
//
FloatRange Blackboard::GetValue( const std::string& keyName, const FloatRange& defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		FloatRange value;
		value.SetFromText(found->second.c_str());
		return value;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the value of they key
//
IntRange Blackboard::GetValue( const std::string& keyName, const IntRange& defaultValue ) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if( found == m_keyValuePairs.end())
		return defaultValue;
	else
	{
		IntRange value;
		value.SetFromText(found->second.c_str());
		return value;
	}
}