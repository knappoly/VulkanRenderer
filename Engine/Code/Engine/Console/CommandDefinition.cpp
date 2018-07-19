#include "Engine/Console/CommandDefinition.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Console/Command.hpp"

//-----------------------------------------------------------------------------------------------
// Static member declaration
std::map<std::string, CommandDefinition*> CommandDefinition::s_commandRegistry;

//-----------------------------------------------------------------------------------------------
// Sets up the command definitions
//
void CommandDefinition::CommandStartup()
{
// 	if(!Command::CommandRunScriptFile("Data/test.script"))
// 	{
// 		DebuggerPrintf("Success");
// 	}
}

//-----------------------------------------------------------------------------------------------
// Deletes the various command definitions
//
void CommandDefinition::CommandShutdown()
{
	CommandDefinition::s_commandRegistry.clear();
}

//-----------------------------------------------------------------------------------------------
// Registers the command with the callback 
//
void CommandDefinition::CommandRegister(const char* name, commandCB cb, const char* helpText)
{
	std::map<std::string, CommandDefinition*>::iterator found = s_commandRegistry.find(std::string(name));
	GUARANTEE_OR_DIE(found == s_commandRegistry.end(), Stringf("Command definition for \"%s\" already exists", name).c_str());

	CommandDefinition* commandDef = new CommandDefinition(name, cb, helpText);
	CommandDefinition::s_commandRegistry[name] = commandDef;
}

//-----------------------------------------------------------------------------------------------
// Displays help text on the console
//
bool CommandDefinition::HelpCommand(Command& cmd)
{
	if(cmd.m_tokens.size() > 2)
	{
		ConsolePrintf("Invalid parameters passed to command:\"help\"");
		return false;
	}

	std::map<std::string, CommandDefinition*>::iterator found = s_commandRegistry.begin();

	CommandDefinition* definition;
	while(found != s_commandRegistry.end())
	{
		definition = found->second;
		ConsolePrintf("%s: %s\n",definition->m_name.c_str(), definition->m_helpText.c_str());
		++found;
	}

	return true;
}

