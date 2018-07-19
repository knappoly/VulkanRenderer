#pragma once
#include <map>
#include <string>
#include <functional>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Command;
typedef std::function<bool(Command&)> commandCB;

//-----------------------------------------------------------------------------------------------
class CommandDefinition
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructor/Destructor
	CommandDefinition(const std::string& name, commandCB cb, const std::string& helpText):m_name(name), m_callback(cb), m_helpText(helpText){}
	~CommandDefinition(){}

	//-----------------------------------------------------------------------------------------------
	// Accessors

	//-----------------------------------------------------------------------------------------------
	// Methods
	static void CommandStartup(); // Allows for setting up commands
	static void CommandShutdown(); // Allows for cleaning up commands
	static void CommandRegister( const char* name, commandCB cb, const char* helpText ); // Registers a command with the system

	//-----------------------------------------------------------------------------------------------
	// Command Callbacks
	static bool HelpCommand(Command& cmd);

	//-----------------------------------------------------------------------------------------------
	// Members
	std::string m_name;
	commandCB	m_callback;
	std::string m_helpText;

	static std::map<std::string, CommandDefinition*> s_commandRegistry;
};

#define COMMAND(name, func_ptr, description) CommandDefinition::CommandRegister(name, func_ptr, description);

