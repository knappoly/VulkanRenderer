#include "Engine/Math/Vector2.hpp"
#include "Engine/Input/XboxStickState.hpp"
//-----------------------------------------------------------------------------------------------
// Initializes the struct
XboxStickState::XboxStickState()
{
	m_stick = Vector2(0.f,0.f);
	m_stickRawValues = Vector2(0.f,0.f);
	m_theta = 0.f;
	m_innerDeadZone = 0.3f;
	m_outerDeadZone = 0.9f;
}
