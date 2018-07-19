//-----------------------------------------------------------------------------------------------
#include "Engine/Core/Time.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Core/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
static Time* g_timeSystem = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Time::Time()
{
	LARGE_INTEGER li;
	::QueryPerformanceFrequency(&li);
	m_frequency = *(uint64_t*) &li;
	m_secondsPerCount = 1.0 / m_frequency;
}

//-----------------------------------------------------------------------------------------------
// Static method to return the current hpc
//
STATIC uint64_t Time::GetPerformanceCounter()
{
	LARGE_INTEGER li;
	::QueryPerformanceCounter(&li);
	return *(uint64_t*) &li;
}

//-----------------------------------------------------------------------------------------------
// Converts hpc to seconds
//
STATIC double Time::HpcToSeconds(const uint64_t hpc)
{
	return (double) hpc * g_timeSystem->m_secondsPerCount;
}

//-----------------------------------------------------------------------------------------------
// Converts seconds to hpc
//
uint64_t Time::SecondsToHpc(float seconds)
{
	return (uint64_t) ( (double) seconds * 1.0 / g_timeSystem->m_secondsPerCount );
}

//-----------------------------------------------------------------------------------------------
// Creates the timesystem instance
//
STATIC void Time::CreateInstance()
{
	if(!g_timeSystem)
	{
		g_timeSystem = new Time();
	}
}

//-----------------------------------------------------------------------------------------------
// Destroys the time instance
//
STATIC void Time::DestroyInstance()
{
	if(g_timeSystem)
	{
		delete g_timeSystem;
		g_timeSystem = nullptr;
	}
}
