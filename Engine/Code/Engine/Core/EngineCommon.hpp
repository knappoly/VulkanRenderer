#pragma once

//-----------------------------------------------------------------------------------------------
// Forward declarations
class Blackboard;

//-----------------------------------------------------------------------------------------------
// Extern declarations
extern Blackboard g_gameConfigBlackboard;

//-----------------------------------------------------------------------------------------------
// Constants
const		int		BITMAP_FONT_TILES_WIDE = 16;
const		int		BITMAP_FONT_TILES_HIGH = 16;
const		float	BITMAP_FONT_DEFAULT_BASE_ASPECT = 1.f;
constexpr	int		NUM_VERTICES_FOR_CIRCLE = 100;
const		float	POINT_RENDER_SCALE = 0.1f;
constexpr	char*	DEV_CONSOLE_OPEN_TONE = "Data/Audio/Tuturu.mp3";
constexpr	char*	DEV_KURISU_SPRITE = "Data/Definitions/kurisu.sprite.xml";
constexpr	char*	DEV_KURISU_ANIM = "Data/Definitions/kurisu.anim.xml";
constexpr	char*	ENGINE_NAME = "K-Nap VKEngine v0.1";

//-----------------------------------------------------------------------------------------------
// Macros
#define UNUSED(x) (void)(x);
#define STATIC 
#define PROFILER_START_FRAME_TEXT	"Frame Start"

// Source from http://www.flipcode.com/archives/FIXME_TODO_Notes_As_Warnings_In_Compiler_Output.shtml
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define PRAGMA(p)  __pragma( p )
#define NOTE( x )  PRAGMA( message(x) )
#define FILE_LINE  NOTE( __FILE__LINE__ )

// THE IMPORANT BITS
#define TODO( x )  NOTE( __FILE__LINE__"\n"           \
        " --------------------------------------------------------------------------------------\n" \
        "|  TODO :   " ##x "\n" \
        " --------------------------------------------------------------------------------------\n" )
#define UNIMPLEMENTED()  TODO( "IMPLEMENT: " QUOTE(__FILE__) " (" QUOTE(__LINE__) ")" "-" __FUNCTION__ ); //GUARANTEE_RECOVERABLE(false, "");
// End from source

//-----------------------------------------------------------------------------------------------
// Typedefs
typedef unsigned int uint;

//-----------------------------------------------------------------------------------------------
// Standalone functions
void	EngineStartup();
void	EngineShutdown();

