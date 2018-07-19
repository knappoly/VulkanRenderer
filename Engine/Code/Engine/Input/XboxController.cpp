#include "Engine/Input/XboxController.hpp"
#include "Engine/Math/MathUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
XboxController::XboxController()
{
	memset(&controllerState, 0,sizeof(controllerState));
	leftStick.m_innerDeadZone = XBOX_STICK_INNER_DEADZONE_RADIUS_PERCENTAGE;
	leftStick.m_outerDeadZone = XBOX_STICK_OUTER_DEADZONE_RADIUS_PERCENTAGE;
	rightStick.m_innerDeadZone = XBOX_STICK_INNER_DEADZONE_RADIUS_PERCENTAGE;
	rightStick.m_outerDeadZone = XBOX_STICK_OUTER_DEADZONE_RADIUS_PERCENTAGE;
}

//-----------------------------------------------------------------------------------------------
// Returns the current state of the controller
//
XINPUT_STATE XboxController::GetControllerState() const
{
	return controllerState;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the controller is connected
// 
bool XboxController::IsConnected()
{
	DWORD result = XInputGetState(controllerNum, &controllerState);
	if(result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the controller id
//
void XboxController::SetControllerNum(int id)
{
	controllerNum = id;
}

//-----------------------------------------------------------------------------------------------
// Applies a rumble to the xbox controller
//
void XboxController::Vibrate(float leftMotor, float rightMotor)
{
	XINPUT_VIBRATION xVibrate;

	memset(&xVibrate, 0, sizeof(xVibrate));

	xVibrate.wLeftMotorSpeed = static_cast<WORD>(leftMotor * 65535.f);
	xVibrate.wRightMotorSpeed = static_cast<WORD>(rightMotor * 65535.f);

	XInputSetState(controllerNum, &xVibrate);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the button is down and held down
//
bool XboxController::IsButtonDown(unsigned char keyCode) const
{
	
	return buttons[keyCode].m_isDown;
}

//-----------------------------------------------------------------------------------------------	
// Returns true if the button was pressed once
//
bool XboxController::WasButtonJustPressed(unsigned char keyCode) const
{
	return buttons[keyCode].m_justPressed;
}

//-----------------------------------------------------------------------------------------------
// Returns true if the button was just released
//
bool XboxController::WasButtonJustReleased(unsigned char keyCode) const
{
	return buttons[keyCode].m_justReleased;
}

//-----------------------------------------------------------------------------------------------
// Returns the current left stick value as float [0,1]
//
Vector2 XboxController::GetLeftThumbStick()
{
	CalculateStickValues(leftStick);
	return leftStick.m_stick;
}

//-----------------------------------------------------------------------------------------------
// Returns the current right stick value as float [0,1]
//
Vector2 XboxController::GetRightThumbStick()
{
	CalculateStickValues(rightStick);
	return rightStick.m_stick;
}

//-----------------------------------------------------------------------------------------------
// Returns the left trigger value as byte [0,255]
//
float XboxController::GetLeftTrigger() const
{
	float temp = static_cast<float>(leftTrigger.m_trigger) * (1.f / 255.f);
	return temp ;
}

//-----------------------------------------------------------------------------------------------
// Returns the right trigger value as byte [0,255]
//
float XboxController::GetRightTrigger() const
{
	float temp = static_cast<float>(rightTrigger.m_trigger) * (1.f / 255.f);
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Calculate the corrected stick values while calibrating deadzone
//
void XboxController::CalculateStickValues(XboxStickState& stick)
{
	float stickXFloat = RangeMapFloat(stick.m_stick.x, XBOX_STICK_RAW_MIN, XBOX_STICK_RAW_MAX, -1.f, 1.f);
	float stickYFloat = RangeMapFloat(stick.m_stick.y, XBOX_STICK_RAW_MIN, XBOX_STICK_RAW_MAX, -1.f, 1.f);

	float polarRSquared = (stickXFloat * stickXFloat) + (stickYFloat * stickYFloat);
	float polarMagnitude = sqrtf(polarRSquared);
	float polarTheta = Atan2Degrees(stickYFloat,stickXFloat);

	float correctedMagnitude = RangeMapFloat(polarMagnitude, stick.m_innerDeadZone, stick.m_outerDeadZone, 0.0f, 1.f);
	correctedMagnitude = ClampFloatZeroToOne(correctedMagnitude);

	float correctedX = correctedMagnitude * CosDegrees(polarTheta);
	float correctedY = correctedMagnitude * SinDegrees(polarTheta);

	
	stick.m_stick = Vector2(correctedX,correctedY);
	stick.m_theta = polarTheta;
	
}

//-----------------------------------------------------------------------------------------------
// Updates the states of the buttons, sticks and triggers
//
void XboxController::UpdateButtonStates()
{
	UpdateXboxButton(	XBOX_BUTTON_A,				XINPUT_GAMEPAD_A  );
	UpdateXboxButton(	XBOX_BUTTON_B,				XINPUT_GAMEPAD_B  );
	UpdateXboxButton(	XBOX_BUTTON_X,				XINPUT_GAMEPAD_X  );
	UpdateXboxButton(	XBOX_BUTTON_Y,				XINPUT_GAMEPAD_Y  );
	UpdateXboxButton(	XBOX_BUTTON_DPAD_UP,        XINPUT_GAMEPAD_DPAD_UP  );
	UpdateXboxButton(	XBOX_BUTTON_DPAD_RIGHT,     XINPUT_GAMEPAD_DPAD_RIGHT  );
	UpdateXboxButton(	XBOX_BUTTON_DPAD_LEFT,      XINPUT_GAMEPAD_DPAD_LEFT  );
	UpdateXboxButton(	XBOX_BUTTON_DPAD_DOWN,      XINPUT_GAMEPAD_DPAD_DOWN  );
	UpdateXboxButton(	XBOX_BUTTON_LEFT_THUMB,		XINPUT_GAMEPAD_LEFT_THUMB);
	UpdateXboxButton(	XBOX_BUTTON_RIGHT_THUMB,	XINPUT_GAMEPAD_RIGHT_THUMB);
	UpdateXboxButton(	XBOX_BUTTON_LEFT_SHOULDER,	XINPUT_GAMEPAD_LEFT_SHOULDER);
	UpdateXboxButton(	XBOX_BUTTON_RIGHT_SHOULDER,	XINPUT_GAMEPAD_RIGHT_SHOULDER);
	UpdateXboxButton(	XBOX_BUTTON_START,			XINPUT_GAMEPAD_START);
	UpdateXboxButton(	XBOX_BUTTON_BACK,			XINPUT_GAMEPAD_BACK);
}

//-----------------------------------------------------------------------------------------------
// Updates the joystick values
//
void XboxController::UpdateStickStates()
{
	leftStick.m_stick  = Vector2( controllerState.Gamepad.sThumbLX,	controllerState.Gamepad.sThumbLY );
	leftStick.m_stickRawValues  = Vector2( controllerState.Gamepad.sThumbLX,	controllerState.Gamepad.sThumbLY );
	rightStick.m_stick = Vector2( controllerState.Gamepad.sThumbRX,	controllerState.Gamepad.sThumbRY );
	rightStick.m_stickRawValues = Vector2( controllerState.Gamepad.sThumbRX,	controllerState.Gamepad.sThumbRY );
}

//-----------------------------------------------------------------------------------------------
// Updates the trigger values
//
void XboxController::UpdateTriggerStates()
{
	leftTrigger.m_trigger  = controllerState.Gamepad.bLeftTrigger;
	leftTrigger.m_triggerNormalized = static_cast<float>(leftTrigger.m_trigger) * (1.f / 255.f);

	rightTrigger.m_trigger = controllerState.Gamepad.bRightTrigger;
	rightTrigger.m_triggerNormalized = static_cast<float>(rightTrigger.m_trigger) * (1.f / 255.f);
}

//-----------------------------------------------------------------------------------------------
// Updates the buttons on the controller
//
void XboxController::UpdateXboxButton(unsigned char keyCode, WORD xinputValue)
{
	if(controllerState.Gamepad.wButtons & xinputValue)
	{
		if(!buttons[keyCode].m_isDown)
			buttons[keyCode].m_justPressed = true;

		buttons[keyCode].m_isDown = true;
	}

	else
	{
		buttons[keyCode].m_isDown = false;
		buttons[keyCode].m_justReleased = true;
	}
}

//-----------------------------------------------------------------------------------------------
// Clears the buttons for the next input
//
void XboxController::ClearButtons()
{
	for(int i = 0; i< XBOX_NUM_BUTTONS; i++)
	{
		buttons[i].m_justPressed = false;
		buttons[i].m_justReleased = false;
	}
}
