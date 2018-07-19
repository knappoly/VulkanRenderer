#pragma once

//-----------------------------------------------------------------------------------------------
//
struct KeyButtonState
{
public:
	KeyButtonState();

	bool m_isDown;
	bool m_justPressed;
	bool m_justReleased;
};
