#pragma once
#include "Engine\Core\Rgba.hpp"
#include "Engine\Renderer\DebugRenderer.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Vector3;
class Vector2;
class Texture;
class AABB2;

//-----------------------------------------------------------------------------------------------
// Standalone Functions

//-----------------------------------------------------------------------------------------------
// Point rendering function prototypes
void DebugRenderPoint( const Vector3& pos, const Rgba& color = Rgba::WHITE ); 
void DebugRenderPoint( float lifeTime, const Vector3& pos, const Rgba& startColor = Rgba::WHITE, const Rgba& endColor = Rgba::WHITE );

//-----------------------------------------------------------------------------------------------
// Line rendering function prototypes
void DebugRenderLine( float lifeTime, const Vector3& start, const Vector3& end, const Rgba& color = Rgba::WHITE, DebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

//-----------------------------------------------------------------------------------------------
// Quad rendering functions prototypes
void DebugRenderQuad( float lifeTime, const Vector3& position, const Vector3& euler, const Vector2& scale, const Texture* texture = nullptr, const Rgba& tint = Rgba::WHITE, 
				DebugRenderMode mode = DEBUG_RENDER_USE_DEPTH);
void DebugRenderGlyph( float lifeTime, const Vector3& position, const Vector3& euler, const Vector2& scale, const Texture* texture = nullptr, const Rgba& tint = Rgba::WHITE,
				DebugRenderMode mode = DEBUG_RENDER_USE_DEPTH);

//-----------------------------------------------------------------------------------------------
// Cube rendering function prototypes
void DebugRenderWireAABB3( float lifeTime, const Vector3& pos, const Vector3& size, const Rgba& color = Rgba::WHITE );

//-----------------------------------------------------------------------------------------------
// Sphere rendering function prototypes
void DebugRenderWireSphere( float lifeTime, const Vector3& pos, float radius, const Rgba& color = Rgba::WHITE ); 

//-----------------------------------------------------------------------------------------------
// Text rendering function prototypes
void DebugRenderTextf( float lifeTime, const Vector3& pos, float cellHeight, const Rgba& color, char const *format, ... ); 
void DebugRenderTextf( float lifeTime, const Vector3& pos, const Vector3& rotation, float cellHeight, const Rgba& color, char const *format, ... ); 
void DebugRenderTextf( float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, ... );
void DebugRenderTextf( float lifeTime, const Vector3& pos, char const *format, ... ); 
void DebugRenderTextf( float lifeTime, const Vector3& pos, float cellHeight, char const *format, ... ); 
void DebugRenderTextf( float lifeTime, const Vector3& pos, const Vector3& rotation, float cellHeight, char const *format, ... ); 
void DebugRenderTextv( float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, va_list varArgs ); 

//-----------------------------------------------------------------------------------------------
// Tag rendering function prototyes (Text thats oriented towards camera)
void DebugRenderTagf( float lifeTime, const Vector3& pos, float cellHeight, const Rgba& color, char const *format, ... ); 
void DebugRenderTagf( float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, ... );
void DebugRenderTagf( float lifeTime, const Vector3& pos, char const *format, ... ); 
void DebugRenderTagf( float lifeTime, const Vector3& pos, float cellHeight, char const *format, ... ); 
void DebugRenderTagv( float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, va_list varArgs ); 

//-----------------------------------------------------------------------------------------------
// 2D text rendering function prototypes
void DebugRenderText2D( float lifeTime, const Vector2& pos, const Vector2& alignment, float cellHeight, const Rgba& color, char const *format, ... ); 

//-----------------------------------------------------------------------------------------------
// 2D Quad rendering function prototypes
void DebugRenderQuad2D( float lifeTime, const AABB2& bounds, const Rgba& color = Rgba::WHITE );
void DebugRenderQuad2D( float lifeTime, const Vector2& position, const Vector2& size, const Rgba& color = Rgba::WHITE );
void DebugRenderQuad2D( float lifeTime, const Vector2& position, float sizeX, float sizeY, const Rgba& color = Rgba::WHITE );

//-----------------------------------------------------------------------------------------------
// Basis rendering function prototypes
void DebugRenderBasis( float lifeTime, const Vector3& pos, const Vector3& rotation, float scale = 1.0f );
void DebugRenderBasis( float lifeTime, const Matrix44& modelMatrix, float scale = 1.f );

//-----------------------------------------------------------------------------------------------
// Log rendering function prototypes 
void DebugRenderLogf( float lifeTime, const Rgba& color, const char* format, ... );
void DebugRenderLogf( float lifeTime, const char* format, ... );
void DebugRenderLogv( float lifeTime, const Rgba& color, const char* format, va_list varArgs );

//-----------------------------------------------------------------------------------------------
// Grid rendering function prototypes
void DebugRenderGrid( float lifeTime, int uniformSize, const Rgba& color = Rgba::WHITE);
void DebugRenderGrid( float lifeTime, const Vector3& right, const Vector3& up, int uniformSize, const Rgba& color = Rgba::WHITE);
