#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Mesh;
class Vector3;
class Matrix44;
class BitmapFont;
typedef unsigned int uint;


//-----------------------------------------------------------------------------------------------
// Standalone functions for Creating LIT objects
Mesh*	CreateQuad( const Vector3& mins, const Vector3& maxs, const Rgba& color = Rgba::WHITE );
Mesh*	CreateTexturedQuad( const Vector3& mins, const Vector3& maxs, const Rgba& color = Rgba::WHITE, const Vector2& texCoordsAtMins = Vector2::ZERO, const Vector2& texCoordsAtMaxs = Vector2::ONE );
Mesh*	CreateQuad3D( const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Rgba& color = Rgba::WHITE );
Mesh*	CreateTexturedQuad3D( const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Rgba& color = Rgba::WHITE, const Vector2& texCoordsAtMins = Vector2::ZERO, const Vector2& texCoordsAtMaxs = Vector2::ONE );
Mesh*	CreateLine( const Vector3& start, const Vector3& end, const Rgba& color = Rgba::WHITE );
Mesh*	CreateLine( const Vector3& start, const Vector3& end, const Rgba& startVertColor, const Rgba& endVertColor = Rgba::WHITE );
Mesh*	CreateCube( const Vector3& center, const Vector3& size, const Rgba& color = Rgba::WHITE, const AABB2& uvTop = AABB2::ZERO_TO_ONE, const AABB2& uvSide = AABB2::ZERO_TO_ONE, const AABB2& uvBottom = AABB2::ZERO_TO_ONE );
Mesh*	CreateUVSphere( const Vector3& position, float radius, uint wedges, uint slices, const Rgba& color = Rgba::WHITE );
Mesh*	CreatePoint( const Vector3& position, const Rgba& color = Rgba::WHITE );
Mesh*	CreateBasisMesh( const Matrix44& basis, float scale = 1.f);
Mesh*	CreateTextMesh2D ( const Vector2& position, const std::string& text, float cellHeight, const BitmapFont* font, const Rgba& tint = Rgba::WHITE, const Vector2& alignment = Vector2::ZERO );
Mesh*	CreateGridMesh( const Vector3& right, const Vector3& up, int uniformSize, const Rgba& color = Rgba::WHITE );
