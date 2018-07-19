#pragma once
#include <string>
#include <vector>
#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Renderer;
class BitmapFont;
class AABB2;
class Command;
class Camera;
class Shader;
class SpriteAnimNew;
class GIFAnimation;

//-----------------------------------------------------------------------------------------------
// To enable colored text in each line 
struct ColoredText
{
	ColoredText(){}
	ColoredText(const std::string str, const Rgba& col)
	{
		text = str;
		color = col;
	}

	std::string text;
	Rgba color;
};

typedef std::vector<ColoredText> ColorStrings;

//-----------------------------------------------------------------------------------------------
class DevConsole
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	DevConsole(float width, float height);
	~DevConsole(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			bool		IsOpen() const;
			AABB2		GetConsoleBounds() const;
			AABB2		GetTextWindowBounds() const;
			AABB2		GetInputBounds() const;
			AABB2		GetKurisuBounds() const;
			void		SetFont(const BitmapFont& font);
			void		SetFontAspect(float aspect) { m_fontAspectScale = aspect; }
			void		SetFontToScreenRatio(float ratio);

	//-----------------------------------------------------------------------------------------------
	// Methods
			void		InitializeCommands();
			void		Update(float deltaSeconds);		 // Handles all input
			void		Render() const; // Renders the display 
			void		RenderConsoleText(Renderer& renderer) const;
			void		RenderCursor(Renderer& renderer) const;
			void		RenderSelection(Renderer& renderer) const;
			void		RenderKurisu() const;
			void		Open(); 
			void		Close(); 
			
			void		InsertInput(const std::string& input);
			void		ClearInput();
			void		RemoveCharAtCursor();
			void		AddTextToConsoleOutput(const std::string& text, const Rgba& color);
			void		ScrollUp();
			void		ScrollDown();
			void		MoveCursorLeft();
			void		MoveCursorRight();
			void		MoveSelectionRight();
			void		MoveSelectionLeft();
			void		AddCommandToHistory(const std::string& commandStr);
			void		GetPrevCommand();
			void		GetNextCommand();

	static	DevConsole* CreateInstance(float width, float height); // Creates the DevConsole
	static	DevConsole* GetInstance();							  // Returns the instance

	//-----------------------------------------------------------------------------------------------
	// Command Callbacks
	static	bool		EchoCommand(Command& cmd);
	static	bool		ClearCommand(Command& cmd);
	static	bool		AddIntCommand(Command& cmd);
	static	bool		SaveLogCommand(Command& cmd);

	//-----------------------------------------------------------------------------------------------
	// Members
private:
			bool			m_isOpen = false;
			float			m_width = 0.f;
			float			m_height = 0.f;
			std::string		m_currentString = "";
			float			m_fontSize = 0.f;
			float			m_fontToScreenRatio = 0.02f;
			int				m_numLinesToDisplay = 32;
			ColorStrings	m_consoleTextBuffer;
			std::string		m_consoleLogArchive = "";
	const	BitmapFont*		m_font = nullptr;
			float			m_textWindowPaddingX = 10.f;
			float			m_textWindowPaddingY = 5.f;
			int				m_cursorIndex = 0;
			Rgba			m_cursorColor = Rgba::RED;
			Rgba			m_selectionColor;
			float			m_fontAspectScale = 1.f;
			float			m_cursorThickness = 5.f;
			size_t			m_scrollIndex = 0;
			int				m_selectionStartIndex = 0;
			Strings			m_commandHistory;
			int				m_historyIndex = -1;
			int				m_maxCommandHistory = 45;
			Camera*			m_consoleCam = nullptr;
			Shader*			m_shader = nullptr;
			size_t			m_openSound;
			GIFAnimation*	m_kurisuGIF = nullptr;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions

// Returns true if the developer console is open
bool IsDevConsoleOpen(); 

// Should add a line of coloured text to the output 
void ConsolePrintf( const Rgba& color, char const *format, ... ); 

// Same as previous, be defaults to a color visible easily on your console
void ConsolePrintf( char const *format, ... ); 

// Same as previous, but takes a va_list as parameter
void ConsolePrintv( const Rgba& color, char const *format, va_list varArgs);
