#pragma once
#include <string>
#include <map>
#include "Engine/Core/StringUtils.hpp"
#include <functional>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Rgba;
class Command;
typedef std::function<bool(Command&)> commandCB; // Command callbacks take a Command.
class Vector3;
class Vector2;

//-----------------------------------------------------------------------------------------------
class Command
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Command( const char* str );
	~Command(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	std::string GetCommmandString() const { return m_commandStr; }
	std::string GetName() const { return m_commandName; }  
	std::string GetNextString();   // would return after each call...
								   // first:  "(255,255,0)""
								   // second: "Hello \"world\""
								   // third+: ""
	bool		GetNextColor(Rgba* out_val);
	bool		GetNextInt(int& out_val);
	bool		GetNextBool(bool& out_val);
	bool		GetNextFloat(float& out_val);
	bool		GetNextChar(char& out_val);
	bool		GetNextVector2(Vector2* out_val);
	bool		GetNextVector3(Vector3* out_val);
	

	//-----------------------------------------------------------------------------------------------
	// Methods
	static bool CommandRun( char const *command ); // Construct command obj and run the callback 
	static bool CommandRunScript( const char* src ); // Execute a script source after parsing
	static bool CommandRunScriptFile( const char* path ); // Execute a script file after parsing

	//-----------------------------------------------------------------------------------------------
	// Members
	Strings				m_tokens;
	std::string			m_commandStr = "";
	std::string			m_commandName = "";
	int					m_currentTokenIndex = 0;
};

