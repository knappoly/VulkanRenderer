#include "Engine/Core/StopWatch.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
StopWatch::StopWatch(Clock* refClock /*= nullptr */)
{
	if(!refClock)
	{
		refClock = Clock::GetMasterClock();
	}
	
	m_reference = refClock;
	m_startHpc = refClock->total.hpc;
}

//-----------------------------------------------------------------------------------------------
// Sets the clock (Maintains elapsed time)
//
void StopWatch::SetClock(Clock* refClock)
{
	m_reference = refClock;
}

//-----------------------------------------------------------------------------------------------
// Sets the timer interval
//
void StopWatch::SetTimer(float seconds)
{
	m_intervalHpc = Time::SecondsToHpc(seconds);
}

//-----------------------------------------------------------------------------------------------
// Stops the timer
//
void StopWatch::Stop()
{
	m_isStopped = true;
}

//-----------------------------------------------------------------------------------------------
// Starts the timer
//
void StopWatch::Start()
{
	if(m_isStopped)
	{
		m_isStopped = false;
		m_startHpc = m_reference->total.hpc;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the elapsed time 
//
float StopWatch::GetElapsedTime() const
{
	if(m_isStopped)
	{
		return 0.f;
	}

	float seconds = (float) Time::HpcToSeconds(m_reference->total.hpc - m_startHpc);
	return seconds;
}

//-----------------------------------------------------------------------------------------------
// Returns the normalized elapsed time
//
float StopWatch::GetNormalizedElapsedTime() const
{
	if(m_isStopped)
	{
		return 0.f;
	}

	uint64_t normalElapsedHpc = (m_reference->total.hpc - m_startHpc) / m_intervalHpc;
	return (float) Time::HpcToSeconds(normalElapsedHpc);
}

//-----------------------------------------------------------------------------------------------
// Returns true if interval has elapsed
//
bool StopWatch::HasElapsed() const
{
	if(m_isStopped)
	{
		return false;
	}
	return m_reference->total.hpc > (m_startHpc + m_intervalHpc);
}

//-----------------------------------------------------------------------------------------------
// Reset the startHpc 
//
void StopWatch::Reset()
{
	m_startHpc = m_reference->total.hpc;
}

//-----------------------------------------------------------------------------------------------
// Checks if atleast one interval elapsed and then resets 
//
bool StopWatch::CheckAndReset()
{
	if(HasElapsed())
	{
		Reset();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
// Checks if an interval elapsed and updates start hpc to account for the elapsed interval
//
bool StopWatch::Decrement()
{
	bool elapsed = HasElapsed();
	if(elapsed)
	{
		m_startHpc = m_startHpc + m_intervalHpc;
	}

	return elapsed;
}

//-----------------------------------------------------------------------------------------------
// Returns the number of times the timer elapsed and decrements each time
//
int StopWatch::DecrementAll()
{
	int count = 0;
	while(HasElapsed())
	{
		Decrement();
		count++;
	}

	return count;
}
