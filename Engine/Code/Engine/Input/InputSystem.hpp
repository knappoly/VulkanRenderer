#pragma once
#include "Engine/Input/XboxController.hpp"
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Input/Mouse.hpp"

//-----------------------------------------------------------------------------------------------
enum KeyCodes
{
	KEYCODE_A		=	'A'	,
	KEYCODE_B		=	'B'	,
	KEYCODE_C		=	'C'	,
	KEYCODE_D		=	'D'	,
	KEYCODE_E		=	'E'	,
	KEYCODE_F		=	'F'	,
	KEYCODE_G		=	'G'	,
	KEYCODE_H		=	'H'	,
	KEYCODE_I		=	'I'	,
	KEYCODE_J		=	'J'	,
	KEYCODE_K		=	'K'	,
	KEYCODE_L		=	'L'	,
	KEYCODE_M		=	'M'	,
	KEYCODE_N		=	'N'	,
	KEYCODE_O		=	'O'	,
	KEYCODE_P		=	'P'	,
	KEYCODE_Q		=	'Q'	,
	KEYCODE_R		=	'R'	,
	KEYCODE_S		=	'S'	,
	KEYCODE_T		=	'T'	,
	KEYCODE_U		=	'U'	,
	KEYCODE_V		=	'V'	,
	KEYCODE_W		=	'W' ,
	KEYCODE_X		=	'X'	,
	KEYCODE_Y		=	'Y'	,
	KEYCODE_Z		=	'Z'	,
	KEYCODE_UP		=	0x26,
	KEYCODE_DOWN	=	0x28,
	KEYCODE_LEFT	=	0x25,
	KEYCODE_RIGHT	=	0x27,
	KEYCODE_ENTER	=	VK_RETURN,
	KEYCODE_ESCAPE	=	VK_ESCAPE,
	KEYCODE_F1		=	VK_F1,
	KEYCODE_F2		=	VK_F2,
	KEYCODE_F3		=	VK_F3,
	KEYCODE_F4		=	VK_F4,
	KEYCODE_F5		=	VK_F5,
	KEYCODE_F6		=	VK_F6,
	KEYCODE_F7		=	VK_F7,
	KEYCODE_F8		=	VK_F8,
	KEYCODE_F9		=	VK_F9,
	KEYCODE_F10		=	VK_F10,
	KEYCODE_F11		=	VK_F11,
	KEYCODE_F12		=	VK_F12,
	KEYCODE_SPACE	=	VK_SPACE,
	KEYCODE_TILDE	=	VK_OEM_3,
	NUM_KEYCODES
};

const int NUM_KEYS = 255;
const int NUM_CONTROLLERS = 4;

//-----------------------------------------------------------------------------------------------
class InputSystem
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors
	InputSystem();
	~InputSystem(){}

	//-----------------------------------------------------------------------------------------------
	// Accessors

	//-----------------------------------------------------------------------------------------------
	// Methods
	void			BeginFrame();
	void			EndFrame();
	void			RunMessagePump();
	void			ProcessKey(unsigned char asKey,int state);
	void			ProcessMouseButtons(size_t wparam);
	void			UpdateKeyboard();
	void			UpdateControllers();
	void			UpdateMouse();

	//-----------------------------------------------------------------------------------------------
	// Static methods
	static	InputSystem*		CreateInstance();
	static	void				DestroyInstance();
	static	InputSystem*		GetInstance();
	static	bool				IsKeyDown( unsigned char keyCode );
	static	bool				WasKeyJustPressed( unsigned char keyCode );
	static	bool				WasKeyJustReleased( unsigned char keyCode );
	static	XboxController&		GetController(int controllerID);
	static	void				ShowCursor( bool show );
	static	Vector2				GetMouseDelta();
	static	Vector2				GetMousePosition();
	static	bool				WasMouseButtonPressed( unsigned char keyCode );
	static  bool				WasMouseButtonReleased( unsigned char keyCode );
	static  bool				IsMouseButtonDown( unsigned char keyCode );
	static	void				SetMousePosition( const Vector2& clientPos );
	static	void				LockMouseToWindow( bool lock );
	static	void				RelockMouse();
	static	void				SetMouseMode( MouseMode mode );
	
protected:
	//-----------------------------------------------------------------------------------------------
	// Members
			KeyButtonState	keyButtons[NUM_KEYS];
			XboxController	controller[NUM_CONTROLLERS];
			Mouse			mouse;
};

