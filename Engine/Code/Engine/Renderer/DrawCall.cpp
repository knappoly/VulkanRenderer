#include "Engine/Renderer/DrawCall.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Material.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Returns the material's sort order
//
int DrawCall::GetSortOrder() const
{
	return m_material->GetSortOrder();
}

//-----------------------------------------------------------------------------------------------
// Returns the renderqueue
//
int DrawCall::GetRenderQueue() const
{
	return m_material->GetRenderQueue();
}
