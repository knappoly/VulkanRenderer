#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"

const char* APP_NAME = "Kevin Nappoly: Basic Triangle Vulkan";	

//-----------------------------------------------------------------------------------------------
// Handles Windows (Win32) messages/events; i.e. the OS is trying to tell us something happened.
//
bool AppMessageHandler( unsigned int wmMessageCode, size_t wParam, size_t lParam ) 
{
	switch( wmMessageCode )
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
		case WM_CLOSE:		
		{
			App::GetInstance()->RequestQuit();
			return false; // "Consumes" this message (tells Windows "okay, we handled it")
		}

		// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
		case WM_KEYDOWN:
		{
			unsigned char asKey = (unsigned char) wParam;
			InputSystem::GetInstance()->ProcessKey(asKey,0);
			return false;
		}

		case WM_KEYUP:
		{
			unsigned char asKey = (unsigned char) wParam;
			InputSystem::GetInstance()->ProcessKey(asKey,1);
			return false;
		}

		case WM_MOUSEWHEEL:
		{
			float wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if(wheelDelta > 0)
			{
				//DevConsole::GetInstance()->ScrollUp();
			}
			if(wheelDelta < 0)
			{
				//DevConsole::GetInstance()->ScrollDown();
			}
		}

		// Handles window resizing and moving
		case WM_MOVE:
		case WM_SIZE:
			InputSystem::GetInstance()->LockMouseToWindow(true);
			break;

		case WM_ACTIVATE:
		{
			bool isActive = (WA_INACTIVE != LOWORD(wParam));
			Window::GetInstance()->SetActiveState(isActive);
			InputSystem::GetInstance()->LockMouseToWindow(isActive);
			if(isActive)
				InputSystem::GetInstance()->ShowCursor(false);
			break;
		}

		// Mouse handling
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			InputSystem::GetInstance()->ProcessMouseButtons(wParam);
			break;
	}

	UNUSED(lParam);
	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return true;
}

//-----------------------------------------------------------------------------------------------
// Handles character input for the console window if it is open
//
bool ConsoleInputHandler( unsigned int wmMessageCode, size_t wParam, size_t lParam )
{
	//if(!IsDevConsoleOpen())
		//return true;

	const int HOTKEY_SHIFT_LEFT = 1;
	const int HOTKEY_SHIFT_RIGHT = 2;
	RegisterHotKey(GetActiveWindow(), HOTKEY_SHIFT_LEFT, MOD_SHIFT, VK_LEFT);
	RegisterHotKey(GetActiveWindow(), HOTKEY_SHIFT_RIGHT, MOD_SHIFT, VK_RIGHT);

	switch (wmMessageCode)
	{
	case WM_CHAR:
	{
		switch(wParam)
		{
		case VK_BACK:
			//DevConsole::GetInstance()->RemoveCharAtCursor();
			return false;

		case VK_RIGHT:
			//DevConsole::GetInstance()->MoveCursorRight();
			return false;

		case VK_LEFT:
			//DevConsole::GetInstance()->MoveCursorLeft();
			return false;
		
		default:
			unsigned char keylog = (unsigned char) wParam;
			//DevConsole::GetInstance()->InsertInput(Stringf("%c",keylog));
			DebuggerPrintf("%c",keylog);
			return false;
		}
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_RIGHT:
			//DevConsole::GetInstance()->MoveCursorRight();
			return false;

		case VK_LEFT:
			//DevConsole::GetInstance()->MoveCursorLeft();
			return false;

		case VK_UP:
			//DevConsole::GetInstance()->GetPrevCommand();
			return false;
		
		case VK_DOWN:
			//DevConsole::GetInstance()->GetNextCommand();
			return false;
		}
	}

	case WM_HOTKEY:
	{
		switch (wParam)
		{
		case HOTKEY_SHIFT_LEFT:
			//DevConsole::GetInstance()->MoveSelectionLeft();
			return false;

		case HOTKEY_SHIFT_RIGHT:
			//DevConsole::GetInstance()->MoveSelectionRight();
			return false;
		}
	}

	}

	UNUSED(lParam);
	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return true;
}

//-----------------------------------------------------------------------------------------------
// Quit command callback
//
// bool QuitCommand(Command& cmd)
// {
// 	App::GetInstance()->RequestQuit();
// 	return true;
// 	UNUSED(cmd);
// }

//-----------------------------------------------------------------------------------------------
// Creates the OpenGL Window
//
void CreateVulkanWindow( float clientAspect )
{
	Window *window = Window::CreateInstance( APP_NAME, clientAspect ); 
	window->AddHandler( AppMessageHandler ); 
	window->AddHandler( ConsoleInputHandler );
}

//-----------------------------------------------------------------------------------------------
// One "frame" of the game.  Generally: Input, Update, Render.  We call this 60+ times per second.
//
void RunFrame()
{
	App::GetInstance()->RunFrame();	
}

//-----------------------------------------------------------------------------------------------
void Initialize()
{
	CreateVulkanWindow( CLIENT_ASPECT );
	VkRenderStartup();
	App::CreateInstance( APP_NAME );
	VKRenderer::GetInstance()->PostStartup();
	
// 	CommandDefinition::CommandRegister("quit", QuitCommand, "Quits the application"); // Registers the quit command
// 	CommandDefinition::CommandStartup();
}

//-----------------------------------------------------------------------------------------------
void Shutdown()
{
	// Destroy the global App instance
	App::DestroyInstance();
}

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	UNUSED( commandLineString );
	UNUSED( applicationInstanceHandle );

	Initialize();

	// Program main loop; keep running frames until it's time to quit
	while( !App::GetInstance()->IsQuitting() ) 
	{
		RunFrame();
	}

	Shutdown();
	return 0;
}


