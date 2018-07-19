#include "Engine/Console/Command.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Console/CommandDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/File/File.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
Command::Command(const char* str)
{
	m_commandStr = str;
	
	// Tokenize the string and check for quotation marks
	StringTokenizer tokenizer(m_commandStr," ");
	tokenizer.SetQuoteCheck(true);
	tokenizer.Tokenize();
	m_tokens = tokenizer.GetTokens();

	m_commandName = m_tokens[0];
}

//-----------------------------------------------------------------------------------------------
// Returns the next string in the token set
//
std::string Command::GetNextString()
{
	m_currentTokenIndex++;
	if(m_currentTokenIndex < m_tokens.size())
		return m_tokens[m_currentTokenIndex];

	return "";
}

//-----------------------------------------------------------------------------------------------
// Returns the next string as a color if parsed correctly else, return false
//
bool Command::GetNextColor(Rgba* out_val)
{
	m_currentTokenIndex++;
	
	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	if(currentString[0] == '(')
	{
		if(currentString.find_last_of(')') == currentString.npos)
		{
			return false; // Unbalanced paranthesis
		}

		currentString.erase(currentString.begin());
		currentString.erase(currentString.size() - 1);
	}

	StringTokenizer colorStr(currentString, ",");
	colorStr.Tokenize();

	if(colorStr.GetTokenCount() > 4 || colorStr.GetTokenCount() < 3)
	{
		return false; // Too little or too many values specified
	}

	Strings colorTokens = colorStr.GetTokens();
	try
	{
		out_val->r = (unsigned char) stoi(colorTokens[0]);
		out_val->g = (unsigned char) stoi(colorTokens[1]);
		out_val->b = (unsigned char) stoi(colorTokens[2]);

		if(colorTokens.size() == 4)
		{
			out_val->a = (unsigned char) stoi(colorTokens[3]);
		}

		else
		{
			out_val->a = (unsigned char) 255;
		}
	}
	catch (const std::exception& )
	{
		return false;
	}

	return true; // Parsed successfully
}

//-----------------------------------------------------------------------------------------------
// Gets the next string as an integer
//
bool Command::GetNextInt(int& out_val)
{
	m_currentTokenIndex++;

	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	try
	{
		out_val = stoi(currentString);
	}
	catch (const std::invalid_argument& )
	{
		// Parse failed. Invalid Argument
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------
// Gets the next string as an bool
//
bool Command::GetNextBool(bool& out_val)
{
	m_currentTokenIndex++;

	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	if(currentString == "true")
	{
		out_val = true;
	}
	else if(currentString == "false")
	{
		out_val = false;
	}
	else
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
// Gets the next string as a float
//
bool Command::GetNextFloat(float& out_val)
{
	m_currentTokenIndex++;

	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	try
	{
		out_val = stof(currentString);
	}
	catch (const std::invalid_argument& )
	{
		// Parse failed. Invalid Argument
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
// Gets the next string as a char
//
bool Command::GetNextChar(char& out_val)
{
	m_currentTokenIndex++;

	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	try
	{
		out_val = (char) stoi(currentString);
	}
	catch (const std::invalid_argument& )
	{
		// Parse failed. Invalid Argument
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
// Gets the next string as a Vec2
//
bool Command::GetNextVector2(Vector2* out_val)
{
	m_currentTokenIndex++;

	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	if(currentString[0] == '(')
	{
		if(currentString.find_last_of(')') == currentString.npos)
		{
			return false; // Unbalanced paranthesis
		}

		currentString.erase(currentString.begin());
		currentString.erase(currentString.size() - 1);
	}

	StringTokenizer vectorStr(currentString, ",");
	vectorStr.Tokenize();

	if(vectorStr.GetTokenCount() > 4 || vectorStr.GetTokenCount() < 3)
	{
		return false; // Too little or too many values specified
	}

	Strings vectorTokens = vectorStr.GetTokens();

	try
	{
		out_val->x = stof(vectorTokens[0]);
		out_val->y = stof(vectorTokens[1]);
	}
	catch (const std::invalid_argument& )
	{
		return false;
	}

	return true; // Parsed successfully
}

//-----------------------------------------------------------------------------------------------
// Gets the next string as a Vec2
//
bool Command::GetNextVector3(Vector3* out_val)
{
	m_currentTokenIndex++;

	if(m_currentTokenIndex >= m_tokens.size())
		return false; // Parse failed. No string available

	std::string& currentString = m_tokens[m_currentTokenIndex];
	if(currentString[0] == '(')
	{
		if(currentString.find_last_of(')') == currentString.npos)
		{
			return false; // Unbalanced paranthesis
		}

		currentString.erase(currentString.begin());
		currentString.erase(currentString.size() - 1);
	}

	StringTokenizer vectorStr(currentString, ",");
	vectorStr.Tokenize();

	if(vectorStr.GetTokenCount() > 3 || vectorStr.GetTokenCount() < 3)
	{
		return false; // Too little or too many values specified
	}

	Strings vectorTokens = vectorStr.GetTokens();

	try
	{
		out_val->x = stof(vectorTokens[0]);
		out_val->y = stof(vectorTokens[1]);
		out_val->z = stof(vectorTokens[2]);
	}
	catch (const std::invalid_argument& )
	{
		return false;
	}

	return true; // Parsed successfully
}

//-----------------------------------------------------------------------------------------------
// Constructs a command object and runs its associated callback
//
bool Command::CommandRun(char const *command)
{
	Command* cmd = new Command(command);
	
	// Clears the input string for next input
	DevConsole::GetInstance()->ClearInput();

	// Search for the command in the registry
	std::map<std::string, CommandDefinition*>::iterator found = CommandDefinition::s_commandRegistry.find(cmd->GetName());

	// if exists, run the call back from the definition object
	if(found != CommandDefinition::s_commandRegistry.end())
	{
		ConsolePrintf(Rgba::WHITE,"%s\n", cmd->GetCommmandString().c_str());
		if(found->second->m_callback(*cmd))
		{
			ConsolePrintf(Rgba::RED,"Command Success\n", cmd->GetName().c_str());
		}
		else
		{
			ConsolePrintf(Rgba::RED,"Command failed\n", cmd->GetName().c_str());
		}
		
		delete cmd;
		cmd = nullptr;
		return false;
	}

	else
	{
		ConsolePrintf(Rgba::RED,"\"%s\" - invalid command\n", cmd->GetName().c_str());
		delete cmd;
		cmd = nullptr;
		return true;
	}
}

//-----------------------------------------------------------------------------------------------
// Construct command objects for each new line encountered and run them
//
bool Command::CommandRunScript(const char* src)
{
	StringTokenizer tokenizer(src, "\n");
	tokenizer.Tokenize();
	Strings commands = tokenizer.GetTokens();

	bool error = true;
	for(std::string cmd : commands)
	{
		error = CommandRun(cmd.c_str()) && error;
	}

	return error;
}

//-----------------------------------------------------------------------------------------------
// Read from file and execute the script file
//
bool Command::CommandRunScriptFile(const char* path)
{
	char* src = (char*) FileReadToNewBuffer(path);
	
	if(src == nullptr)
	{
		ConsolePrintf(Rgba::RED, "\"%s\" could not be loaded", path);
		return true;
	}

	return CommandRunScript(src);
}
