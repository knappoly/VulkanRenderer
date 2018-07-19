#pragma once
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef bool (*windowsMessageHandlerCB)( unsigned int msg, size_t wParam, size_t lParam );

//-----------------------------------------------------------------------------------------------
class Window
{
private:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Window( const char* title, float clientAspect );
public:
	~Window();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void* GetHandle() const { return m_hWnd; }
	bool  IsActive() const { return m_isActive; }
	void  SetActiveState( bool state ) { m_isActive = state; }

	//-----------------------------------------------------------------------------------------------
	// Methods
			void	AddHandler( windowsMessageHandlerCB cb );
			void	RemoveHandler( windowsMessageHandlerCB cb );
			void	RemoveAllHandlers(); 
	static  Window*	CreateInstance( const char* title, float aspect );
	static	Window* GetInstance();
	static	void	DestroyInstance();

	//-----------------------------------------------------------------------------------------------
	// Members
	void*									m_hWnd;
	std::vector<windowsMessageHandlerCB>	m_messageHandlers; 
	float									m_width;
	float									m_height;
	bool									m_isActive = true;
};

