#pragma once
#include <cstdint>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Clock;

//-----------------------------------------------------------------------------------------------
class StopWatch
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	StopWatch( Clock* ref = nullptr );
	~StopWatch(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void	SetClock( Clock* refClock );
	void	SetTimer( float seconds );
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void	Stop();
	void	Start();
	float	GetElapsedTime() const;
	float	GetNormalizedElapsedTime() const;
	bool	HasElapsed() const;
	void	Reset();
	bool	CheckAndReset();
	bool	Decrement();
	int		DecrementAll();

	
	//-----------------------------------------------------------------------------------------------
	// Members
	Clock*		m_reference;
	uint64_t	m_startHpc;
	uint64_t	m_intervalHpc;
	bool		m_isStopped = false;
};

