#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Input/InputSystem.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
static InputSystem* g_theInput = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
InputSystem::InputSystem()
{
	for(int i =0; i<NUM_KEYS;i++)
	{
		keyButtons[i].m_isDown = false;
		keyButtons[i].m_justPressed = false;
		keyButtons[i].m_justReleased = false;
	}
}
//-----------------------------------------------------------------------------------------------
// Runs at the start of each frame
//
void InputSystem::BeginFrame()
{
	UpdateKeyboard();
	UpdateControllers();
	UpdateMouse();
	RunMessagePump();
}

//-----------------------------------------------------------------------------------------------
// Runs at the end of each frame
//
void InputSystem::EndFrame()
{
	for(int i = 0; i < NUM_CONTROLLERS; i++)
		controller[i].ClearButtons();

	mouse.ClearButtons();
}

//-----------------------------------------------------------------------------------------------
// Processes all Windows messages (WM_xxx) for this app that have queued up since last frame.
// For each message in the queue, our WindowsMessageHandlingProcedure (or "WinProc") function
//	is called, telling us what happened (key up/down, minimized/restored, gained/lost focus, etc.)
//
void InputSystem::RunMessagePump()
{
	MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage ); // This tells Windows to call our "WindowsMessageHandlingProcedure" function
	}
}
//-----------------------------------------------------------------------------------------------
// Processes Keyboard events and manipulates the corresponding states
//
void InputSystem::ProcessKey(unsigned char asKey, int state)
{
	switch (state)
	{
	case 0:
		if(!keyButtons[asKey].m_isDown)
			keyButtons[asKey].m_justPressed = true;
		else 
			keyButtons[asKey].m_justPressed = false;

		keyButtons[asKey].m_isDown = true;
		break;

	case 1:
		keyButtons[asKey].m_justReleased = true;
		keyButtons[asKey].m_isDown = false;
		break;
	default:
		break;
	}
	
}

//-----------------------------------------------------------------------------------------------
// Processes the mouse button
//
void InputSystem::ProcessMouseButtons(size_t wparam)
{
	mouse.ProcessMouseButtons(wparam);
}

//-----------------------------------------------------------------------------------------------
// Resets the keystates to accept further input
//
void InputSystem::UpdateKeyboard()
{
	for(int i=0; i<NUM_KEYS; i++)
	{
		keyButtons[i].m_justReleased = false;
		keyButtons[i].m_justPressed = false;
	}
}

//-----------------------------------------------------------------------------------------------
// Reset the controller state to accept further input
//
void InputSystem::UpdateControllers()
{
	for(int i = 0; i<NUM_CONTROLLERS; i++)
	{
		controller[i].SetControllerNum(i);
		if(controller[i].IsConnected())
		{
			controller[i].UpdateButtonStates();
			controller[i].UpdateStickStates();
			controller[i].UpdateTriggerStates();
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Updates the mouse
//
void InputSystem::UpdateMouse()
{
	mouse.UpdateMouse();
}

//-----------------------------------------------------------------------------------------------
// Creates the input system instance
//
InputSystem* InputSystem::CreateInstance()
{
	if(g_theInput == nullptr)
	{
		g_theInput = new InputSystem();
	}

	return g_theInput;
}

//-----------------------------------------------------------------------------------------------
// Destroys the input system instance
//
void InputSystem::DestroyInstance()
{
	if(g_theInput != nullptr)
	{
		delete g_theInput;
		g_theInput = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the input system instance
//
InputSystem* InputSystem::GetInstance()
{
	return g_theInput;
}

//-----------------------------------------------------------------------------------------------
// Returns true if Key is down or held-down
//
bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return (g_theInput->keyButtons[keyCode].m_isDown);
}

//-----------------------------------------------------------------------------------------------
// Returns true if key was just pressed (Single Press)
//
bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return (g_theInput->keyButtons[keyCode].m_justPressed);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the button was just released
//
bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return (g_theInput->keyButtons[keyCode].m_justReleased);
		
}

//-----------------------------------------------------------------------------------------------
// Returns the XboxController reference 
//
XboxController& InputSystem::GetController(int controllerID)
{
	return g_theInput->controller[controllerID];
}

//-----------------------------------------------------------------------------------------------
// Sets the cursor show mode
//
void InputSystem::ShowCursor(bool show)
{
	g_theInput->mouse.ShowCursor(show);
}

//-----------------------------------------------------------------------------------------------
// Returns the mouse delta
//
Vector2 InputSystem::GetMouseDelta()
{
	return g_theInput->mouse.GetMouseDelta();
}

//-----------------------------------------------------------------------------------------------
// Returns the client position of the mouse
//
Vector2 InputSystem::GetMousePosition()
{
	return g_theInput->mouse.GetMouseClientPosition();
}

//-----------------------------------------------------------------------------------------------
// Returns true if the mouse button was just pressed
//
bool InputSystem::WasMouseButtonPressed(unsigned char keyCode)
{
	return g_theInput->mouse.WasButtonJustPressed(keyCode);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the mouse button was just released
//
bool InputSystem::WasMouseButtonReleased(unsigned char keyCode)
{
	return g_theInput->mouse.WasButtonJustReleased(keyCode);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the mouse button is held down
//
bool InputSystem::IsMouseButtonDown(unsigned char keyCode)
{
	return g_theInput->mouse.IsButtonDown(keyCode);
}

//-----------------------------------------------------------------------------------------------
// Sets the mouse position with the client position specified
//
void InputSystem::SetMousePosition(const Vector2& clientPos)
{
	g_theInput->mouse.SetMouseClientPosition(clientPos);
}

//-----------------------------------------------------------------------------------------------
// Locks the mouse to the openGL window
//
void InputSystem::LockMouseToWindow(bool lock)
{
	g_theInput->mouse.LockMouseToWindow(lock);
}

//-----------------------------------------------------------------------------------------------
// Relocks the mouse to window if lock is enabled
//
void InputSystem::RelockMouse()
{
	g_theInput->mouse.RelockMouse();
}

//-----------------------------------------------------------------------------------------------
// Sets the mouse mode to either Absolute or Relative
//
void InputSystem::SetMouseMode(MouseMode mode)
{
	g_theInput->mouse.SetMouseMode(mode);
}

