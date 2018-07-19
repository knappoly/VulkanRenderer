#include "Engine/Core/Vertex.hpp"
#include "Engine/Core/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals

// VERTEX_3DPCU
STATIC const VertexAttribute Vertex_3DPCU::s_attributes[]
{
	VertexAttribute("POSITION", RT_FLOAT,			VKRT_VECTOR3,			3,	false,	(uint) offsetof(Vertex_3DPCU, m_position)),
	VertexAttribute("COLOR",	RT_FLOAT,			VKRT_VECTOR4,			4,	false,	(uint) offsetof(Vertex_3DPCU, m_color)),
	VertexAttribute("UV",		RT_FLOAT,			VKRT_VECTOR2,			2,	false,	(uint) offsetof(Vertex_3DPCU, m_UVs))
};

STATIC const VertexLayout Vertex_3DPCU::s_layout(Vertex_3DPCU::s_attributes, sizeof(Vertex_3DPCU), 3);
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
VertexLit::VertexLit(const VertexBuilder& builder)
{
	m_position = builder.m_position;
	m_color = builder.m_color;
	m_UVs = builder.m_UV;
	m_normal = builder.m_normal;
	m_tangent = builder.m_tangent;
}

//-----------------------------------------------------------------------------------------------
// VERTEXLIT
STATIC const VertexAttribute VertexLit::s_attributes[]
{
	VertexAttribute("POSITION", RT_FLOAT,			VKRT_FLOAT,			3,	false,	(uint) offsetof(VertexLit, m_position)),
	VertexAttribute("COLOR",	RT_FLOAT,			VKRT_VECTOR4,		4,	false,	(uint) offsetof(VertexLit, m_color)),
	VertexAttribute("UV",		RT_FLOAT,			VKRT_VECTOR2,		2,	false,	(uint) offsetof(VertexLit, m_UVs)),
	VertexAttribute("NORMAL",	RT_FLOAT,			VKRT_VECTOR3,		3,	false,	(uint) offsetof(VertexLit, m_normal)),
	VertexAttribute("TANGENT",	RT_FLOAT,			VKRT_VECTOR4,		4,	false,	(uint) offsetof(VertexLit, m_tangent))
};

STATIC const VertexLayout VertexLit::s_layout(VertexLit::s_attributes, sizeof(VertexLit), 5);
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
Vertex_3DPCU::Vertex_3DPCU(const VertexBuilder& builder)
{
	m_position = builder.m_position;
	m_color = builder.m_color;
	m_UVs = builder.m_UV;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VertexAttribute::VertexAttribute(const char* handle, RenderType type, VKRenderType vkType, int count, bool normalized, size_t offset)
	: m_handle(handle)
	, m_type(type)
	, m_vkType(vkType)
	, m_memberOffset(offset)
	, m_isNormalized(normalized)
	, m_elementCount(count)
{
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VertexLayout::VertexLayout(const VertexAttribute* attribs, int stride, int count)
{
	m_stride = stride;

	const VertexAttribute* attrib = attribs;
	for( int index = 0; index < count; ++index )
	{
		m_attributes.push_back(attrib);
		attrib++;
	}
}
