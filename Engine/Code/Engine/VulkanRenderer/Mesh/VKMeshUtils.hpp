#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Vector3;
class Matrix44;
class BitmapFont;
typedef unsigned int uint;
class VKMesh;


//-----------------------------------------------------------------------------------------------
// Standalone functions for Creating LIT objects
VKMesh*	CreateQuad( const Vector3& mins, const Vector3& maxs, const Rgba& color = Rgba::WHITE );
VKMesh*	CreateTexturedQuad( const Vector3& mins, const Vector3& maxs, const Rgba& color = Rgba::WHITE, const Vector2& texCoordsAtMins = Vector2::ZERO, const Vector2& texCoordsAtMaxs = Vector2::ONE );
VKMesh*	CreateQuad3D( const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Rgba& color = Rgba::WHITE );
VKMesh*	CreateTexturedQuad3D( const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Rgba& color = Rgba::WHITE, const Vector2& texCoordsAtMins = Vector2::ZERO, const Vector2& texCoordsAtMaxs = Vector2::ONE );
VKMesh*	CreateLine( const Vector3& start, const Vector3& end, const Rgba& color = Rgba::WHITE );
VKMesh*	CreateLine( const Vector3& start, const Vector3& end, const Rgba& startVertColor, const Rgba& endVertColor = Rgba::WHITE );
VKMesh*	CreateCube( const Vector3& center, const Vector3& size, const Rgba& color = Rgba::WHITE, const AABB2& uvTop = AABB2::ZERO_TO_ONE, const AABB2& uvSide = AABB2::ZERO_TO_ONE, const AABB2& uvBottom = AABB2::ZERO_TO_ONE );
VKMesh*	CreateUVSphere( const Vector3& position, float radius, uint wedges, uint slices, const Rgba& color = Rgba::WHITE );
VKMesh*	CreatePoint( const Vector3& position, const Rgba& color = Rgba::WHITE );
VKMesh*	CreateBasisMesh( const Matrix44& basis, float scale = 1.f);
VKMesh*	CreateTextMesh2D ( const Vector2& position, const std::string& text, float cellHeight, const BitmapFont* font, const Rgba& tint = Rgba::WHITE, const Vector2& alignment = Vector2::ZERO );
VKMesh*	CreateGridMesh( const Vector3& right, const Vector3& up, int uniformSize, const Rgba& color = Rgba::WHITE );
