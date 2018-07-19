#pragma once
#include <cstdint>
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
struct TimeUnit
{
	uint64_t ms;
	double seconds;
	uint64_t hpc;
};

//-----------------------------------------------------------------------------------------------
class Clock
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Clock( Clock* parent = nullptr ); // Not const because we add child to the parent (if needed)
	~Clock();
	Clock( const Clock& ) = delete; // Disables copy constructor
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			double				GetDeltaSeconds() const { return frame.seconds; }
			double				GetTime() const { return total.seconds; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void				AddChild( Clock& child );
			void				BeginFrame();
			void				EndFrame();
			
			void				PauseClock() { m_isPaused = true; }
			void				ResumeClock() { m_isPaused = false; }
			void				SetTimeScale( float scale ) { m_scale = scale; }
			void				ResetTimeScale() { m_scale = 1.f; }

	static	void				CreateMasterClock();
	static	Clock*				GetMasterClock();
	static 	void				DestroyMasterClock();
	static	double				GetMasterDeltaSeconds();
	static	float				GetMasterFPS();

	//-----------------------------------------------------------------------------------------------
	// Members
			TimeUnit			frame;
			TimeUnit			total;
			
private:
			std::vector<Clock*> m_childClocks;
			Clock*				m_parent = nullptr;
			uint64_t			m_prevHpc;
			float				m_scale = 1.f;
			bool				m_isPaused = false;
			uint64_t			m_frameCounter = 0;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
void	ClockSystemStartup();
void	ClockSystemShutdown();