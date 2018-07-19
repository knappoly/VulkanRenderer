#include "Engine/Renderer/DebugRenderUtils.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Camera.hpp"
#include <stdarg.h>
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Shader.hpp"

//-----------------------------------------------------------------------------------------------
// Draws a point on the screen
//
void DebugRenderPoint(const Vector3& pos, const Rgba& color /*= Rgba::WHITE */)
{
	DebugRenderPoint(0.f, pos, color, color);
}

//-----------------------------------------------------------------------------------------------
// Draws a point on the screen
//
void DebugRenderPoint(float lifeTime, const Vector3& pos, const Rgba& startColor, const Rgba& endColor)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = startColor;
	options->endColor = endColor;
	options->lifetime = lifeTime;
	dRender->DebugRenderPoint(pos, *options);
}

//-----------------------------------------------------------------------------------------------
// Draws a line point on the screen
//
void DebugRenderLine(float lifeTime, const Vector3& start, const Vector3& end, const Rgba& color /*= Rgba::WHITE*/, DebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;
	options->mode = mode;
	dRender->DebugRenderLineSegment(start, end, *options);
}

//-----------------------------------------------------------------------------------------------
// Draws a quad on the screen
//
void DebugRenderQuad(float lifeTime, const Vector3& position, const Vector3& euler, const Vector2& scale, const Texture* texture /*= nullptr*/,  
	const Rgba& tint /*= Rgba::WHITE*/, DebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH*/)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = tint;
	options->endColor = tint;
	options->lifetime = lifeTime;
	options->mode = mode;

	dRender->DebugRenderQuad(position, euler, scale, *options, false, texture);
}

//-----------------------------------------------------------------------------------------------
// Draws a camera oriented quad on the screen
//
void DebugRenderGlyph(float lifeTime, const Vector3& position, const Vector3& euler, const Vector2& scale, const Texture* texture /*= nullptr*/, const Rgba& tint /*= Rgba::WHITE*/, DebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH*/)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = tint;
	options->endColor = tint;
	options->lifetime = lifeTime;
	options->mode = mode;

	dRender->DebugRenderQuad(position, euler, scale, *options, true, texture);
}

//-----------------------------------------------------------------------------------------------
// Draws a wireframe cube on the screen
//
void DebugRenderWireAABB3(float lifeTime, const Vector3& pos, const Vector3& size, const Rgba& color /*= Rgba::WHITE */)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;

	dRender->DebugRenderAABB3(pos, size, FILLMODE_WIRE, *options);
}

//-----------------------------------------------------------------------------------------------
// Draws a wire UV sphere on the screen
//
void DebugRenderWireSphere(float lifeTime, const Vector3& pos, float radius, const Rgba& color /*= Rgba::WHITE */)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;

	dRender->DebugRenderSphere(pos, radius, 32, 16, FILLMODE_WIRE, *options);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextv(float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, va_list varArgs)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;

	const int MAX_LENGTH = 2048;
	char buffer[MAX_LENGTH];
	vsnprintf_s( buffer, MAX_LENGTH, _TRUNCATE, format, varArgs );
	buffer[MAX_LENGTH - 1] = '\0';

	dRender->DebugRender3DText(pos, rotation, buffer, cellHeight, *options, alignment, false);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextf(float lifeTime, const Vector3& pos, float cellHeight, const Rgba& color, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTextv(lifeTime, pos, Vector3::ZERO, Vector3(0.5f, 0.f), cellHeight, color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextf(float lifeTime, const Vector3& pos, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTextv(lifeTime, pos, Vector3::ZERO, Vector3(0.5f, 0.f), 1.f, Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextf(float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTextv(lifeTime, pos, rotation, alignment, cellHeight, color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextf(float lifeTime, const Vector3& pos, float cellHeight, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTextv(lifeTime, pos, Vector3::ZERO, Vector3(0.5f, 0.f), cellHeight, Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextf(float lifeTime, const Vector3& pos, const Vector3& rotation, float cellHeight, const Rgba& color, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTextv(lifeTime, pos, rotation, Vector3(0.5f, 0.f), cellHeight, color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D text in the scene
//
void DebugRenderTextf(float lifeTime, const Vector3& pos, const Vector3& rotation, float cellHeight, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTextv(lifeTime, pos, rotation, Vector3(0.5f, 0.f), cellHeight, Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D tag on the screen 
//
void DebugRenderTagf(float lifeTime, const Vector3& pos, float cellHeight, const Rgba& color, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTagv(lifeTime, pos, Vector3::ZERO, Vector3(0.5f, 0.f), cellHeight, color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D tag on the screen
//
void DebugRenderTagf(float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTagv(lifeTime, pos, rotation, alignment, cellHeight, color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a tag on the screen
//
void DebugRenderTagf(float lifeTime, const Vector3& pos, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTagv(lifeTime, pos, Vector3::ZERO, Vector3(0.5f, 0.f), 1.f, Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a tag on the screen
//
void DebugRenderTagf(float lifeTime, const Vector3& pos, float cellHeight, char const *format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderTagv(lifeTime, pos, Vector3::ZERO, Vector3(0.5f, 0.f), cellHeight, Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Draws a 3D tag on the screen 
//
void DebugRenderTagv(float lifeTime, const Vector3& pos, const Vector3& rotation, const Vector2& alignment, float cellHeight, const Rgba& color, const char* format, va_list varArgs)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;

	const int MAX_LENGTH = 2048;
	char buffer[MAX_LENGTH];
	vsnprintf_s( buffer, MAX_LENGTH, _TRUNCATE, format, varArgs );
	buffer[MAX_LENGTH - 1] = '\0';

	dRender->DebugRender3DText(pos, rotation, buffer, cellHeight, *options, alignment, true);
}

//-----------------------------------------------------------------------------------------------
// Draws a 2d text on the screen (In screenspace)
//
void DebugRenderText2D(float lifeTime, const Vector2& pos, const Vector2& alignment, float cellHeight, const Rgba& color, char const *format, ...)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;
	options->mode = DEBUG_RENDER_IGNORE_DEPTH;

	const int MAX_LENGTH = 2048;
	char buffer[MAX_LENGTH];
	va_list varArgs;
	va_start(varArgs, format);
	vsnprintf_s( buffer, MAX_LENGTH, _TRUNCATE, format, varArgs );
	va_end(varArgs);
	buffer[MAX_LENGTH - 1] = '\0';

	dRender->DebugRender2DText(pos, buffer, cellHeight, *options, alignment);
}

//-----------------------------------------------------------------------------------------------
// Draws a 2d quad on the screen (In screenspace)
//
void DebugRenderQuad2D(float lifeTime, const AABB2& bounds, const Rgba& color)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->startColor = color;
	options->endColor = color;
	options->lifetime = lifeTime;
	options->mode = DEBUG_RENDER_IGNORE_DEPTH;

	dRender->DebugRender2DQuad(bounds, *options);
}

//-----------------------------------------------------------------------------------------------
// Draws a 2d quad on the screen (In screenspace)
//
void DebugRenderQuad2D(float lifeTime, const Vector2& position, const Vector2& size, const Rgba& color /*= Rgba::WHITE */)
{
	DebugRenderQuad2D(lifeTime, AABB2(position, size.x, size.y), color);
}

//-----------------------------------------------------------------------------------------------
// Draws a 2D quad on the screen (screenspace)
//
void DebugRenderQuad2D(float lifeTime, const Vector2& position, float sizeX, float sizeY, const Rgba& color /*= Rgba::WHITE */)
{
	DebugRenderQuad2D(lifeTime, AABB2(position, sizeX, sizeY), color);
}

//-----------------------------------------------------------------------------------------------
// Draws a basis on the screen
//
void DebugRenderBasis(float lifeTime, const Vector3& pos, const Vector3& rotation, float scale /*= 1.0f */)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->lifetime = lifeTime;

	Transform localTransform;
	localTransform.Translate(pos);
	localTransform.SetEulerAngles(rotation);
	dRender->DebugRenderBasis(localTransform.GetWorldMatrix(), scale, *options);
}

//-----------------------------------------------------------------------------------------------
// Draws a basis on the screen using the model matrix
//
void DebugRenderBasis(float lifeTime, const Matrix44& modelMatrix, float scale /*= 1.f */)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->lifetime = lifeTime;

	dRender->DebugRenderBasis(modelMatrix, scale, *options);
}

//-----------------------------------------------------------------------------------------------
// Renders text on the log
//
void DebugRenderLogf(float lifeTime, const Rgba& color, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderLogv(lifeTime, color, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Renders text on the log 
//
void DebugRenderLogf(float lifeTime, const char* format, ...)
{
	va_list varArgs;
	va_start(varArgs, format);
	DebugRenderLogv(lifeTime, Rgba::WHITE, format, varArgs);
	va_end(varArgs);
}

//-----------------------------------------------------------------------------------------------
// Renders text on the log
//
void DebugRenderLogv(float lifeTime, const Rgba& color, const char* format, va_list varArgs)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->lifetime = lifeTime;
	options->startColor = color;
	options->endColor = color;
	
	const int MAX_LENGTH = 2048;
	char buffer[MAX_LENGTH];
	vsnprintf_s( buffer, MAX_LENGTH, _TRUNCATE, format, varArgs );
	buffer[MAX_LENGTH - 1] = '\0';

	dRender->DebugRenderLog(buffer, *options);
}

//-----------------------------------------------------------------------------------------------
// Renders a grid in the scene
//
void DebugRenderGrid(float lifeTime, const Vector3& right, const Vector3& up, int uniformSize, const Rgba& color /*= Rgba::WHITE*/)
{
	DebugRenderer* dRender = DebugRenderer::GetInstance();
	DebugRenderOptions* options = new DebugRenderOptions();
	options->lifetime = lifeTime;
	options->startColor = color;
	options->endColor = color;

	dRender->DebugRenderGrid(right, up, uniformSize, *options);
}

//-----------------------------------------------------------------------------------------------
// Renders a grid assuming XZ plane
//
void DebugRenderGrid(float lifeTime, int uniformSize, const Rgba& color /*= Rgba::WHITE*/)
{
	DebugRenderGrid(lifeTime, Vector3::RIGHT, Vector3::FORWARD, uniformSize, color);
}
