#pragma once
#include "Engine/Enumerations/DrawPrimitiveType.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint;

//-----------------------------------------------------------------------------------------------
struct DrawInstruction
{
	bool				m_useIndices;
	size_t				m_startIndex;
	uint				m_elementCount;
	DrawPrimitiveType	m_drawType;
};

