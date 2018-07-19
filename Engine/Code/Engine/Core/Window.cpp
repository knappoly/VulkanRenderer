#include "Engine/Core/Window.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"

#define GAME_WINDOW_CLASS (TEXT( "Simple Window Class" ))

static Window* g_Window = nullptr; 

//-----------------------------------------------------------------------------------------------
// Handles Windows (Win32) messages/events; i.e. the OS is trying to tell us something happened.
// This function is called by Windows whenever we ask it for notifications
// #SD1ToDo: We will move this function to a more appropriate place when we're ready
//
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	bool run_default = true; 
	Window *window = Window::GetInstance(); 
	if (nullptr != window) {
		for (windowsMessageHandlerCB iter : window->m_messageHandlers) {
			run_default = iter( wmMessageCode, wParam, lParam ) && run_default;  
		}
	}


	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	if (run_default) {
		return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
	} else {
		return 0; 
	}
}

//-----------------------------------------------------------------------------------------------
// Registers the window class paramerters
//
static void RegisterWindowClass() 
{
	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = GAME_WINDOW_CLASS;
	RegisterClassEx( &windowClassDescription );
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Window::Window(const char* title, float clientAspect)
{
	RegisterWindowClass(); 

	// #SD1ToDo: Add support for fullscreen mode (requires different window style flags than windowed mode)
	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );
	float desktopWidth = (float)( desktopRect.right - desktopRect.left );
	float desktopHeight = (float)( desktopRect.bottom - desktopRect.top );
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	constexpr float maxClientFractionOfDesktop = 0.90f;
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	if( clientAspect > desktopAspect )
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * clientAspect;
	}

	m_height = clientHeight;
	m_width = clientWidth;
	
	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;
	clientRect.left = (int) clientMarginX;
	clientRect.right = clientRect.left + (int) clientWidth;
	clientRect.top = (int) clientMarginY;
	clientRect.bottom = clientRect.top + (int) clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, title, -1, windowTitle, sizeof( windowTitle ) / sizeof( windowTitle[ 0 ] ) );
	HWND hwnd = CreateWindowEx(
		windowStyleExFlags,
		GAME_WINDOW_CLASS,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		GetModuleHandle( NULL ),
		NULL );

	ShowWindow( hwnd, SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );

	m_hWnd = (void*)hwnd; 
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Window::~Window()
{
	::DestroyWindow( (HWND)m_hWnd ); 

	::UnregisterClass( GAME_WINDOW_CLASS, GetModuleHandle(NULL) ); 
	m_hWnd = nullptr; 
	g_Window = nullptr; 
}

//-----------------------------------------------------------------------------------------------
// Adds an input handler to the window
//
void Window::AddHandler(windowsMessageHandlerCB cb)
{
	m_messageHandlers.push_back( cb ); 
}

//-----------------------------------------------------------------------------------------------
// Removes the handler from the window
//
void Window::RemoveHandler(windowsMessageHandlerCB cb)
{
	for(windowsMessageHandlerCB iter: m_messageHandlers)
	{
		if(iter == cb)
		{
			iter = nullptr;
			iter = m_messageHandlers[m_messageHandlers.size() - 1];
			m_messageHandlers.pop_back();
			return;
		}
	}
	GUARANTEE_RECOVERABLE(false, "Window handler could not be found and removed.");
}

//-----------------------------------------------------------------------------------------------
// Removes all registered handlers for the window
//
void Window::RemoveAllHandlers()
{
	m_messageHandlers.clear();
}

//-----------------------------------------------------------------------------------------------
// Creates a window
//
STATIC Window* Window::CreateInstance(const char* title, float aspect)
{
	if (g_Window == nullptr) {
		g_Window = new Window( title, aspect ); 
	}
	return g_Window; 
}

//-----------------------------------------------------------------------------------------------
// Returns the created window's handle
//
STATIC Window* Window::GetInstance()
{
	return g_Window;
}

//-----------------------------------------------------------------------------------------------
// Destroys the window instance
//
STATIC void Window::DestroyInstance()
{
	delete g_Window;
	g_Window = nullptr;
}
