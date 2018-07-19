#pragma once
#include <Engine/Math/Vector2.hpp>

//-----------------------------------------------------------------------------------------------
//
struct XboxStickState
{
public:
	XboxStickState();
	Vector2		m_stick;
	Vector2		m_stickRawValues;
	float		m_theta;
	float		m_innerDeadZone;
	float		m_outerDeadZone;
};