#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
static Clock* g_masterClock;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Clock::Clock( Clock* parent /*= nullptr */)
{
	m_parent = parent;

	if(m_parent)
	{
		m_parent->AddChild(*this);
	}
	else
	{
		Time::CreateInstance(); // No parent means master clock which is only done once
	}
	m_prevHpc = Time::GetPerformanceCounter();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Clock::~Clock()
{
	if(!m_parent)
	{
		Time::DestroyInstance(); // Destroy time when master clock is destroyed
	}
}

//-----------------------------------------------------------------------------------------------
// Adds the child clock to the child list
//
void Clock::AddChild(Clock& child)
{
	m_childClocks.push_back(&child);
}

//-----------------------------------------------------------------------------------------------
// Begins the frame for the clock
//
void Clock::BeginFrame()
{
	uint64_t curHpc = Time::GetPerformanceCounter();
	uint64_t elapsed = curHpc - m_prevHpc;
	m_frameCounter++;

	if(m_isPaused)
	{
		elapsed = 0;
	}
	else
	{
		elapsed = (uint64_t) ((float) elapsed * m_scale); // Applies time scale if not paused
	}

	double elapsedSeconds = Time::HpcToSeconds(elapsed);
	frame.seconds = elapsedSeconds;
	frame.hpc = elapsed;
	frame.ms = (uint64_t) elapsedSeconds * 1000;

	total.seconds += frame.seconds;
	total.hpc += frame.hpc;
	total.ms += frame.ms;

	for(Clock* iter : m_childClocks)
	{
		if(m_parent) // Only game clocks should have timescale
		{
			iter->m_scale = m_scale; // Scales all child clocks (if any)
		}
		iter->BeginFrame(); // Advances every child clock (if any)
	}

	m_prevHpc = curHpc;
}

//-----------------------------------------------------------------------------------------------
// End frame for the clock
//
void Clock::EndFrame()
{

}

//-----------------------------------------------------------------------------------------------
// Creates the master clock
//
void Clock::CreateMasterClock()
{
	if(!g_masterClock)
	{
		g_masterClock = new Clock();
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the master clock
//
Clock* Clock::GetMasterClock()
{
	return g_masterClock;
}

//-----------------------------------------------------------------------------------------------
// Destroys the master clock
//
void Clock::DestroyMasterClock()
{
	if(g_masterClock)
	{
		delete g_masterClock;
		g_masterClock = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the master clock's delta seconds
//
double Clock::GetMasterDeltaSeconds()
{
	if(g_masterClock)
		return g_masterClock->frame.seconds;

	else
		return 0.0;
}

//-----------------------------------------------------------------------------------------------
// Returns the FPS using the master clock
//
float Clock::GetMasterFPS()
{
	return 1.f / (float) Clock::GetMasterDeltaSeconds();
}

//-----------------------------------------------------------------------------------------------
// Starts up the master clock
//
void ClockSystemStartup()
{
	Clock::CreateMasterClock();
}

//-----------------------------------------------------------------------------------------------
// Shuts down the master clock
//
void ClockSystemShutdown()
{
	Clock::DestroyMasterClock();
}
