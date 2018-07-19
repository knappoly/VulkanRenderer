#include "Engine/Console/DevConsole.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/CommandDefinition.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <stdarg.h>
#include <stdio.h>
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/File/File.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/GIFAnimation.hpp"

static DevConsole* g_theConsole = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
DevConsole::DevConsole(float width, float height)
{
	m_openSound = AudioSystem::GetInstance()->CreateOrGetSound(DEV_CONSOLE_OPEN_TONE);
	m_width = width;
	m_height = height;
	m_fontSize = m_fontToScreenRatio * m_height;
	m_numLinesToDisplay = 32;
	m_selectionColor = Rgba::FromBytes(100,200,0,150);
	m_consoleCam = new Camera();
	m_consoleCam->SetColorTarget(Renderer::GetInstance()->m_defaultColorTarget);
	
	m_shader = new Shader(Renderer::GetInstance()->CreateOrGetShaderProgram("diffuse"));
	m_shader->SetColorBlending(BLEND_OP_ADD, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
	m_shader->DisableDepth();

	m_kurisuGIF = Renderer::GetInstance()->AcquireGIFInstance("Data/Images/Kurisu.gif");
	m_kurisuGIF->PlayFromStart();
	InitializeCommands();
}

//-----------------------------------------------------------------------------------------------
// Returns true if the console is open
//
bool DevConsole::IsOpen() const
{
	return m_isOpen;
}

//-----------------------------------------------------------------------------------------------
// Returns the bounds of the total console window
//
AABB2 DevConsole::GetConsoleBounds() const
{
	return AABB2(Vector2::ZERO, Vector2(m_width, m_height));
}

//-----------------------------------------------------------------------------------------------
// Returns the bounds for the text window bounds 
//
AABB2 DevConsole::GetTextWindowBounds() const
{
	AABB2 bounds(GetConsoleBounds());
	bounds.mins.x += m_textWindowPaddingX;
	bounds.maxs.x -= m_textWindowPaddingX;
	bounds.mins.y = GetInputBounds().mins.y + m_fontSize;
	bounds.maxs.y -= m_textWindowPaddingY;

	return bounds;
}

//-----------------------------------------------------------------------------------------------
// Returns the bounds for the text input box
//
AABB2 DevConsole::GetInputBounds() const
{
	Vector2 mins(GetConsoleBounds().mins.x + m_textWindowPaddingX, 0.f);
	Vector2 maxs(m_width, m_fontSize);
	return AABB2(mins, maxs);
}

//-----------------------------------------------------------------------------------------------
// Returns the bounds for the Kurisu box
//
AABB2 DevConsole::GetKurisuBounds() const
{
	AABB2 textBounds = GetTextWindowBounds();
	Vector2 mins(0.70f * textBounds.maxs.x, 0.80f * textBounds.maxs.y);
	Vector2 maxs(textBounds.maxs);

	return AABB2(mins, maxs);
}

//-----------------------------------------------------------------------------------------------
// Sets the font for the console
//
void DevConsole::SetFont(const BitmapFont& font)
{
	m_font = &font;
}

//-----------------------------------------------------------------------------------------------
// Sets the font to screen ratio and adjusts font size
//
void DevConsole::SetFontToScreenRatio(float ratio)
{
	m_fontToScreenRatio = ratio;
	m_fontSize *= m_fontToScreenRatio;
}

//-----------------------------------------------------------------------------------------------
// Initializes the initial dev commands
//
void DevConsole::InitializeCommands()
{
	COMMAND("echo", EchoCommand, "Prints the string on the console");
	COMMAND("clear", ClearCommand, "Clears the console window");
	COMMAND("help", CommandDefinition::HelpCommand, "Displays help text on the console window");
	COMMAND("addint", AddIntCommand, "Adds the integers specified");
	COMMAND("savelog", SaveLogCommand, "Writes a log file to disk")
}

//-----------------------------------------------------------------------------------------------
// Updates the developer console 
//
void DevConsole::Update(float deltaSeconds)
{
	m_kurisuGIF->Update(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------
// Renders the developer console
//
void DevConsole::Render() const
{
	Renderer& renderer = *Renderer::GetInstance();
	renderer.SetCamera(m_consoleCam);
	renderer.SetShader(m_shader);
	renderer.SetOrtho(Matrix44::MakeOrtho3D(0.f, m_width, 0.f, m_height, -1.f, 1.f));
	renderer.SetDefaultTexture();
	renderer.SetDefaultMaterial();
	renderer.DrawAABB2(GetConsoleBounds(), Rgba::FromBytes(0,0,100,50));
	renderer.DrawAABB2(GetTextWindowBounds(), Rgba::FromBytes(100,100,100,50));
	renderer.DrawAABB2(GetInputBounds(), Rgba::FromBytes(0,0,0,50));
	
	RenderCursor(renderer);
	RenderSelection(renderer);
	RenderConsoleText(renderer);
	RenderKurisu();
}

//-----------------------------------------------------------------------------------------------
// Renders the tokenized string buffer
//
void DevConsole::RenderConsoleText(Renderer& renderer) const
{
	Vector2 drawMins = GetInputBounds().mins;
	
	// Render input string
	renderer.DrawText2D(drawMins, m_currentString, m_fontSize, m_font, Rgba::WHITE, m_fontAspectScale);
	
	// Render the console text
	int lineIndex = 0;
	
	for(ColorStrings::const_reverse_iterator line = m_consoleTextBuffer.rbegin() + m_scrollIndex; line != m_consoleTextBuffer.rend() && lineIndex < m_numLinesToDisplay; ++line)
	{
		// Reads in reverse order to print the text from bottom -> top
		drawMins.y += m_fontSize;
		renderer.DrawText2D(drawMins, line->text, m_fontSize, m_font, line->color, m_fontAspectScale);
		lineIndex++;
	}
}

//-----------------------------------------------------------------------------------------------
// Renders the cursor on the input box
//
void DevConsole::RenderCursor(Renderer& renderer) const
{
	float fontWidth = m_fontSize *  m_fontAspectScale;
	AABB2 cursorBounds = GetInputBounds();

	cursorBounds.mins.x += fontWidth * m_cursorIndex;
	cursorBounds.maxs.x = cursorBounds.mins.x + m_cursorThickness;

	renderer.DrawAABB2(cursorBounds, m_cursorColor);
}

//-----------------------------------------------------------------------------------------------
// Renders the text selection hightlight
//
void DevConsole::RenderSelection(Renderer& renderer) const
{
	if(m_selectionStartIndex == m_cursorIndex)
		return; // No selection

	AABB2 selectionBounds = GetInputBounds();
	selectionBounds.maxs.x = selectionBounds.mins.x;

	float fontWidth = m_fontSize *  m_fontAspectScale;
	
	if(m_selectionStartIndex > m_cursorIndex)
	{
		selectionBounds.maxs.x = selectionBounds.mins.x;
		selectionBounds.mins.x += fontWidth * m_selectionStartIndex;
		selectionBounds.maxs.x += fontWidth * m_cursorIndex;
	}

	else
	{
		selectionBounds.mins.x += fontWidth * m_selectionStartIndex;
		selectionBounds.maxs.x += fontWidth * m_cursorIndex;
		
	}
	
	renderer.DrawAABB2(selectionBounds, m_cursorColor);
}

//-----------------------------------------------------------------------------------------------
// Renders the Kurisu box
//
void DevConsole::RenderKurisu() const
{
	Renderer* rend = Renderer::GetInstance();
	
	rend->SetTexture(0, (Texture*) m_kurisuGIF->GetCurrentTexture());
	rend->DrawAABB2(GetKurisuBounds());
}

//-----------------------------------------------------------------------------------------------
// Opens the developer console
//
void DevConsole::Open()
{
	InputSystem::LockMouseToWindow(false);
	InputSystem::ShowCursor(true);
	InputSystem::GetInstance()->SetMouseMode(MOUSE_ABSOLUTE);
	AudioSystem::GetInstance()->PlaySound(m_openSound);
	m_isOpen = true;
}

//-----------------------------------------------------------------------------------------------
// Closes the developer console
//
void DevConsole::Close()
{
	InputSystem::LockMouseToWindow(false);
	InputSystem::ShowCursor(false);
	InputSystem::GetInstance()->SetMouseMode(MOUSE_RELATIVE);
	m_isOpen = false;
}

//-----------------------------------------------------------------------------------------------
// Appends the input character to the current string
//
void DevConsole::InsertInput(const std::string& input)
{
	if(input == "\r") // Carriage return should trigger command
	{
		AddCommandToHistory(m_currentString);
		Command::CommandRun(m_currentString.c_str());
		return;
	}

	if(input == "`")
	{
		return;
	}

	m_currentString.insert(m_cursorIndex, input);
	m_cursorIndex++;

	m_selectionStartIndex = m_cursorIndex;
}

//-----------------------------------------------------------------------------------------------
// Clears the input box
//
void DevConsole::ClearInput()
{
	m_currentString.clear();
	m_cursorIndex = 0;
	m_selectionStartIndex = 0;
}

//-----------------------------------------------------------------------------------------------
// Removes one character from the end of the input string
//
void DevConsole::RemoveCharAtCursor()
{
	if(m_currentString.size() > 0)
	{
		m_currentString.erase(m_cursorIndex - 1, 1);
		m_cursorIndex--;
		m_selectionStartIndex = m_cursorIndex;
	}
	
}

//-----------------------------------------------------------------------------------------------
// Adds text to the console output display
//
void DevConsole::AddTextToConsoleOutput(const std::string& text, const Rgba& color)
{
	m_consoleTextBuffer.push_back(ColoredText(text,color));
}

//-----------------------------------------------------------------------------------------------
// Scrolls the text box up
//
void DevConsole::ScrollUp()
{
	if(m_consoleTextBuffer.size() > m_numLinesToDisplay)
	{
		if(m_scrollIndex < (m_consoleTextBuffer.size() - m_numLinesToDisplay))
			m_scrollIndex++;
		
	}
}

//-----------------------------------------------------------------------------------------------
// Scrolls the text box down
//
void DevConsole::ScrollDown()
{
	if(m_consoleTextBuffer.size() > m_numLinesToDisplay)
	{
		if(m_scrollIndex > 0)
			m_scrollIndex--;

		else
			m_scrollIndex = 0;
	}
}

//-----------------------------------------------------------------------------------------------
// Moves the cursor to the left
//
void DevConsole::MoveCursorLeft()
{
	if(m_cursorIndex > 0)
	{
		m_cursorIndex--;
	}
	m_selectionStartIndex = m_cursorIndex;
}

//-----------------------------------------------------------------------------------------------
// Moves the cursor to the right
//
void DevConsole::MoveCursorRight()
{
	if(m_cursorIndex < m_currentString.size())
	{
		m_cursorIndex++;
	}
	m_selectionStartIndex = m_cursorIndex;
}

//-----------------------------------------------------------------------------------------------
// Moves selection end index to the right
//
void DevConsole::MoveSelectionRight()
{
	if(m_cursorIndex < m_currentString.size())
	{
		m_cursorIndex++;
	}
}

//-----------------------------------------------------------------------------------------------
// Moves selection end index to the left
//
void DevConsole::MoveSelectionLeft()
{
	if(m_cursorIndex > 0)
	{
		m_cursorIndex--;
	}

}

//-----------------------------------------------------------------------------------------------
// Adds the command to history vector and avoid duplicates in history
//
void DevConsole::AddCommandToHistory(const std::string& commandStr)
{
	Strings::iterator found = std::find(m_commandHistory.begin(), m_commandHistory.end(), commandStr);
	if(found != m_commandHistory.end())
	{
		m_commandHistory.erase(found); // If duplicate found. Delete that entry
	}

	if(m_commandHistory.size() > m_maxCommandHistory)
	{
		m_commandHistory.pop_back();
	}

	m_commandHistory.insert(m_commandHistory.begin(), commandStr);
	m_historyIndex = -1;
}

//-----------------------------------------------------------------------------------------------
// Updates currentString with the previous command string
//
void DevConsole::GetPrevCommand()
{
	if(m_historyIndex < ((int) m_commandHistory.size() - 1))
	{
		m_historyIndex++;
		m_currentString = m_commandHistory[m_historyIndex];
		m_cursorIndex = (int) m_currentString.size();
		m_selectionStartIndex = m_cursorIndex;
	}

	
}

//-----------------------------------------------------------------------------------------------
// Updates the current string with the next command string
//
void DevConsole::GetNextCommand()
{
	if(m_historyIndex > 0)
	{
		m_historyIndex--;
		m_currentString = m_commandHistory[m_historyIndex];
		m_cursorIndex = (int) m_currentString.size();
		m_selectionStartIndex = m_cursorIndex;
	}
	

}	

//-----------------------------------------------------------------------------------------------
// Creates the instance for the developer console
//
DevConsole* DevConsole::CreateInstance(float width, float height)
{
	if(!g_theConsole)
	{
		g_theConsole = new DevConsole(width, height);
	}

	return g_theConsole;
}

//-----------------------------------------------------------------------------------------------
// Returns the instance for the developer console
//
DevConsole* DevConsole::GetInstance()
{
	return g_theConsole;
}							

//-----------------------------------------------------------------------------------------------
// Command callback for the "echo" command
//
bool DevConsole::EchoCommand(Command& cmd)
{
	Rgba color;
	if(cmd.GetNextColor(&color))
	{
		std::string theString = cmd.GetNextString();
		ConsolePrintf(color, "%s", theString.c_str());
	}
	else
	{
		ConsolePrintf(Rgba::RED, "Invalid arguments in \"echo\"");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
// Command callback for the "clear" command
//
bool DevConsole::ClearCommand(Command& cmd)
{
	if(cmd.m_tokens.size() > 1)
	{
		ConsolePrintf("\"clear\" does not accept arguments");
		return false;
	}

	// Write existing console text to a log member
	for(ColoredText str : g_theConsole->m_consoleTextBuffer)
	{
		g_theConsole->m_consoleLogArchive.append(str.text);
	}

	g_theConsole->m_consoleTextBuffer.clear();
	return true;
}

//-----------------------------------------------------------------------------------------------
// Adds integers and prints the value on the console
//
bool DevConsole::AddIntCommand(Command& cmd)
{
	int sum = 0;
	int curVal;
	for(int intIndex = 1; intIndex < cmd.m_tokens.size(); intIndex++)
	{
		if(cmd.GetNextInt(curVal))
		{
			sum += curVal;
		}
		else
		{
			ConsolePrintf("Argument error: \"%d\" in the \"addint command\"");
			return false;
			break; 
		}
	}

	ConsolePrintf("The sum is: %d", sum);
	return true;
}

//-----------------------------------------------------------------------------------------------
// Saves a log file with all the console text
//
bool DevConsole::SaveLogCommand(Command& cmd)
{
	if(cmd.m_tokens.size() > 2)
	{
		ConsolePrintf("\"savelog\" - Too many arguments");
		return false;
	}

	for(ColoredText str : g_theConsole->m_consoleTextBuffer)
	{
		// Write existing console data to archiveString
		g_theConsole->m_consoleLogArchive.append(str.text);
	}

	std::string fileName = cmd.GetNextString();
	std::string& logData = DevConsole::GetInstance()->m_consoleLogArchive;
	if(FileWriteToNewFile(fileName.c_str(), logData.c_str(), logData.size()))
	{
		ConsolePrintf("Log written to \"%s\" successfully.",fileName.c_str());
	}

	else
	{
		ConsolePrintf("Could not write to \"%s\"",fileName.c_str());
		return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the dev console is open
//
bool IsDevConsoleOpen()
{
	if(g_theConsole)
		return g_theConsole->IsOpen(); 

	else 
		return false;
}

//-----------------------------------------------------------------------------------------------
// Writes text to console with the color specified
//
void ConsolePrintv(const Rgba& color, const char* format, va_list varArgs)
{
	const int MAX_LENGTH = 2048;
	char buffer[MAX_LENGTH];

	vsnprintf_s(buffer, MAX_LENGTH, _TRUNCATE, format, varArgs);
	buffer[MAX_LENGTH - 1] = '\0';

	DevConsole::GetInstance()->AddTextToConsoleOutput(buffer, color);
}


//-----------------------------------------------------------------------------------------------
// Writes text to console
//
void ConsolePrintf(const Rgba& color, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	ConsolePrintv(color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Writes text to console with default color
//
void ConsolePrintf(char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	ConsolePrintv(Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

