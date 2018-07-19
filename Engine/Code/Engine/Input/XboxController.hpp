#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>			//Note: If this is not done here xinput_state doesn't work
#include <Xinput.h>
#pragma comment(lib, "xinput9_1_0")
#include "Engine/Input/XboxStickState.hpp"
#include "Engine/Input/XboxTriggerState.hpp"
#include "Engine/Input/KeyButtonState.hpp"

enum XboxControllerButtons
{
	XBOX_BUTTON_A,				//=	XINPUT_GAMEPAD_A,
	XBOX_BUTTON_B,				 //=	XINPUT_GAMEPAD_B,
	XBOX_BUTTON_X,				 //=	XINPUT_GAMEPAD_X,
	XBOX_BUTTON_Y,				 //=	XINPUT_GAMEPAD_Y,
	XBOX_BUTTON_LEFT_THUMB,		 //=	XINPUT_GAMEPAD_LEFT_THUMB,
	XBOX_BUTTON_RIGHT_THUMB,	 //=	XINPUT_GAMEPAD_RIGHT_THUMB,
	XBOX_BUTTON_LEFT_SHOULDER,	 //=	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XBOX_BUTTON_RIGHT_SHOULDER,  //=	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XBOX_BUTTON_START,			//=	XINPUT_GAMEPAD_START,
	XBOX_BUTTON_BACK,		    //=	XINPUT_GAMEPAD_BACK,
	XBOX_BUTTON_DPAD_UP,		//=	XINPUT_GAMEPAD_DPAD_UP,
	XBOX_BUTTON_DPAD_RIGHT,		//=	XINPUT_GAMEPAD_DPAD_RIGHT,
	XBOX_BUTTON_DPAD_LEFT,		//=	XINPUT_GAMEPAD_DPAD_LEFT,
	XBOX_BUTTON_DPAD_DOWN,		//=	XINPUT_GAMEPAD_DPAD_DOWN,
	XBOX_NUM_BUTTONS			
};

const float XBOX_STICK_INNER_DEADZONE_RADIUS_PERCENTAGE	= 0.3f;
const float XBOX_STICK_OUTER_DEADZONE_RADIUS_PERCENTAGE	= 0.9f;
const float XBOX_STICK_RAW_MAX							= 32767.f;
const float XBOX_STICK_RAW_MIN							= -32768.f;

class XboxController
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors
	XboxController();
	~XboxController(){}

	//-----------------------------------------------------------------------------------------------
	// Accessors
	XINPUT_STATE	GetControllerState() const;
	bool			IsConnected();
	bool			IsButtonDown(unsigned char keyCode) const;
	bool			WasButtonJustPressed(unsigned char keyCode) const;
	bool			WasButtonJustReleased(unsigned char keyCode) const;
	Vector2			GetLeftThumbStick();
	Vector2			GetRightThumbStick();
	float			GetLeftTrigger() const;
	float			GetRightTrigger() const;
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void			CalculateStickValues(XboxStickState& stick);
	void			UpdateButtonStates();
	void			UpdateStickStates();
	void			UpdateTriggerStates();
	void			UpdateXboxButton(unsigned char keyCode, WORD xinputValue);
	void			ClearButtons();
	void			SetControllerNum(int id);
	void			Vibrate(float leftMotor, float rightMotor);

private:
	XINPUT_STATE		controllerState;
	int					controllerNum;
	XboxStickState		leftStick;
	XboxStickState		rightStick;
	XboxTriggerState	leftTrigger;
	XboxTriggerState	rightTrigger;
	KeyButtonState		buttons[XBOX_NUM_BUTTONS];

};