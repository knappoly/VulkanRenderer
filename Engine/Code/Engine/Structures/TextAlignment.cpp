#include "Engine/Structures/TextAlignment.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Text Alignment Constants
const Vector2 TextAlignment::ALIGN_CENTER = Vector2(0.5f,0.5f);
const Vector2 TextAlignment::ALIGN_BOTTOM_RIGHT = Vector2(1.f,0.f);
const Vector2 TextAlignment::ALIGN_BOTTOM_CENTER = Vector2(0.5f,0.f);
const Vector2 TextAlignment::ALIGN_BOTTOM_LEFT = Vector2::ZERO;
const Vector2 TextAlignment::ALIGN_TOP_RIGHT = Vector2(1.f,1.f);
const Vector2 TextAlignment::ALIGN_TOP_CENTER = Vector2(0.5f,1.f);
const Vector2 TextAlignment::ALIGN_TOP_LEFT = Vector2(0.f,1.f);
const Vector2 TextAlignment::ALIGN_MID_RIGHT = Vector2(1.f,0.5f);
const Vector2 TextAlignment::ALIGN_MID_LEFT = Vector2(0.f,0.5f);