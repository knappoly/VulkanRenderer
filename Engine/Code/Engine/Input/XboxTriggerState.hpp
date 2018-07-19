#pragma once

//-----------------------------------------------------------------------------------------------
//
struct XboxTriggerState
{
public:
	XboxTriggerState();
	unsigned char m_trigger;
	float m_triggerNormalized;
};