#include "Engine/Input/Mouse.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/Window.hpp"

//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
Mouse::Mouse()
{

}

//-----------------------------------------------------------------------------------------------
// Returns the mouse delta 
//
Vector2 Mouse::GetMouseDelta() const
{
	return m_positionThisFrame - m_positionLastFrame;
}

//-----------------------------------------------------------------------------------------------
// Returns the screen position of the mouse
//
Vector2 Mouse::GetMouseScreenPosition() const
{
	POINT screenPos;
	::GetCursorPos( &screenPos );

	return Vector2( (float) screenPos.x, (float) screenPos.y);
}

//-----------------------------------------------------------------------------------------------
// Returns the client position of the mouse
//
Vector2 Mouse::GetMouseClientPosition() const
{
	POINT screenPos;
	::GetCursorPos( &screenPos );

	::ScreenToClient((HWND) Window::GetInstance()->GetHandle(), &screenPos);
	POINT clientPos = screenPos; 

	return Vector2( (float) clientPos.x, (float) clientPos.y);
}

//-----------------------------------------------------------------------------------------------
// Returns the center of the window
//
Vector2 Mouse::GetCenterOfClientWindow() const
{
	Window* window = Window::GetInstance();
	Vector2 clientPos;
	clientPos.x = (window->m_width * 0.5f);
	clientPos.y = (window->m_height * 0.5f);

	return clientPos;
}

//-----------------------------------------------------------------------------------------------
// Sets the mouse position with the screen position provided
//
void Mouse::SetMouseScreenPosition(const Vector2& screenPos)
{
	::SetCursorPos( (int) screenPos.x, (int) screenPos.y );
}

//-----------------------------------------------------------------------------------------------
// Sets the mouse position with the client position provided
//
void Mouse::SetMouseClientPosition(const Vector2& clientPos)
{
	POINT desktopPos;
	desktopPos.x = (int) clientPos.x;
	desktopPos.y = (int) clientPos.y;
	::ClientToScreen((HWND) Window::GetInstance()->GetHandle(), &desktopPos);

	// Need to convert to desktop pos to set the position
	SetMouseScreenPosition(Vector2((float) desktopPos.x, (float) desktopPos.y));
}

//-----------------------------------------------------------------------------------------------
// Updates the position values of the mouse based on mouse relative or absolute
//
void Mouse::UpdateMouse()
{
	if(Window::GetInstance()->IsActive())
	{
		// handling data as client positions
			// Absolute mouse mode
		m_positionLastFrame = m_positionThisFrame;
		m_positionThisFrame = GetMouseClientPosition();

		if (m_mouseMode == MOUSE_RELATIVE)
		{
			m_positionLastFrame = GetCenterOfClientWindow();
			SetMouseClientPosition(m_positionLastFrame);
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Process the mouse button input
//
void Mouse::ProcessMouseButtons(size_t wparam)
{
	UpdateMouseButton(	MOUSE_LMB,		MK_LBUTTON,		wparam  );
	UpdateMouseButton(	MOUSE_RMB,		MK_RBUTTON,		wparam  );
	UpdateMouseButton(	MOUSE_MMB,		MK_MBUTTON,		wparam  );
}

//-----------------------------------------------------------------------------------------------
// Updates the state of the mouse button
//
void Mouse::UpdateMouseButton(unsigned char buttonCode, size_t buttonValue, size_t wparam)
{
	if(wparam & buttonValue)
	{
		if(!m_mouseButtons[buttonCode].m_isDown)
		{
			m_mouseButtons[buttonCode].m_justPressed = true;
		}
		m_mouseButtons[buttonCode].m_isDown = true;
	}
	else
	{
		m_mouseButtons[buttonCode].m_isDown = false;
		m_mouseButtons[buttonCode].m_justReleased = true;
	}
}

//-----------------------------------------------------------------------------------------------
// Clears the buttons for the next input
//
void Mouse::ClearButtons()
{
	for(int index =0; index < MOUSE_NUM_BUTTONS; ++index)
	{
		m_mouseButtons[index].m_justPressed = false;
		m_mouseButtons[index].m_justReleased = false;
	}
}

//-----------------------------------------------------------------------------------------------
// Enables or disables showing of the cursor
//
void Mouse::ShowCursor(bool show)
{
	m_isShowingCursor = show;
	::ShowCursor(m_isShowingCursor);
}

//-----------------------------------------------------------------------------------------------
// Locks the mouse to the window
//
void Mouse::LockMouseToWindow(bool lock)
{
	if(Window::GetInstance()->IsActive())
	{
		m_isMouseLocked = lock;
		if (!m_isMouseLocked)
		{
			// Unlocks cursor
			::ClipCursor(nullptr);
			return;
		}

		Window* window = Window::GetInstance();
		RECT clientRegion;
		::GetClientRect((HWND)window->GetHandle(), &clientRegion); // Top left is 0,0

		POINT offset;
		offset.x = offset.y = 0;
		// Converts 0,0 to desktop position to determine the offset of the window
		::ClientToScreen((HWND)window->GetHandle(), &offset);

		clientRegion.left += offset.x;
		clientRegion.right += offset.x;
		clientRegion.top += offset.y;
		clientRegion.bottom += offset.y;

		// Locks the cursor to the bounds of the window
		::ClipCursor(&clientRegion);
	}
}

//-----------------------------------------------------------------------------------------------
// Relocks the mouse to window if lock is enabled
//
void Mouse::RelockMouse()
{
	if(m_isMouseLocked)
	{
		LockMouseToWindow(true);
	}
}

//-----------------------------------------------------------------------------------------------
// Returns true if the button was pressed for a frame
//
bool Mouse::WasButtonJustPressed(unsigned char keyCode) const
{
	return m_mouseButtons[keyCode].m_justPressed;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the button was just released
//
bool Mouse::WasButtonJustReleased(unsigned char keyCode) const
{
	return m_mouseButtons[keyCode].m_justReleased;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the button is held down
//
bool Mouse::IsButtonDown(unsigned char keyCode) const
{
	return m_mouseButtons[keyCode].m_isDown;
}
