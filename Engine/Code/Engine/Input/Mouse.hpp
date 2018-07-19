#pragma once
#include "Engine\Math\Vector2.hpp"
#include "Engine\Input\KeyButtonState.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations

//-----------------------------------------------------------------------------------------------
enum MouseButtons
{
	MOUSE_LMB,
	MOUSE_RMB,
	MOUSE_MMB,
	MOUSE_NUM_BUTTONS
};
//-----------------------------------------------------------------------------------------------
enum MouseMode
{
	MOUSE_ABSOLUTE,
	MOUSE_RELATIVE
};

//-----------------------------------------------------------------------------------------------
class Mouse
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Mouse();
	~Mouse(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Vector2 GetMouseDelta() const;
	Vector2	GetMouseScreenPosition() const;
	Vector2	GetMouseClientPosition() const;
	Vector2	GetCenterOfClientWindow() const;

	void	SetMouseScreenPosition( const Vector2& desktopPos );
	void	SetMouseClientPosition( const Vector2& clientPos );
	void	SetMouseMode( MouseMode mode ) { m_mouseMode = mode; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void	UpdateMouse();
	void	ProcessMouseButtons( size_t wparam );
	void	UpdateMouseButton( unsigned char buttonCode, size_t buttonValue, size_t wparam);
	void	ClearButtons();
	void	ShowCursor( bool show );
	void	LockMouseToWindow( bool lock );
	void	RelockMouse();
	bool	WasButtonJustPressed( unsigned char keyCode ) const;
	bool	WasButtonJustReleased( unsigned char keyCode ) const;
	bool	IsButtonDown( unsigned char keyCode ) const;

	//-----------------------------------------------------------------------------------------------
	// Members
private:
	bool			m_isShowingCursor = true;
	bool			m_isMouseLocked = true;
	MouseMode		m_mouseMode = MOUSE_ABSOLUTE;
	Vector2			m_positionThisFrame;
	Vector2			m_positionLastFrame;
	KeyButtonState	m_mouseButtons[MOUSE_NUM_BUTTONS];
};

