#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Vector2.hpp"
#include <string>
#include <vector>
#include "Engine/Renderer/External/GL/glcorearb.h"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint;


//-----------------------------------------------------------------------------------------------
enum RenderType
{
	RT_FLOAT,			
	RT_UNSIGNED_BYTE,	
	RT_UNSIGNED_INT,		
	RT_INT				
};

//-----------------------------------------------------------------------------------------------
enum VKRenderType
{
	VKRT_INT,
	VKRT_FLOAT,
	VKRT_UNSIGNED_INT,
	VKRT_UNSIGNED_BYTE,
	VKRT_VECTOR2,
	VKRT_VECTOR3,
	VKRT_VECTOR4,
};

//-----------------------------------------------------------------------------------------------
struct VertexBuilder
{
	//-----------------------------------------------------------------------------------------------
	// Constructors
	VertexBuilder(){}
	VertexBuilder( const Vector3& position, const Rgba& color, const Vector2& uv ) : m_position(position),  m_color(color), m_UV(uv){}
	VertexBuilder( const Vector3& position, const Rgba& color, const Vector2& uv, const Vector3& normal ) : m_position(position), m_color(color), m_UV(uv), m_normal(normal){}

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3 m_position;
	Rgba	m_color;
	Vector2 m_UV;
	Vector3 m_normal;
	Vector4 m_tangent = Vector4(Vector3::RIGHT, 1.f);
};

//-----------------------------------------------------------------------------------------------
struct VertexAttribute // Single vertex attrib data description
{

	//-----------------------------------------------------------------------------------------------
	// Constructors
	VertexAttribute( const char* handle, RenderType type, VKRenderType vkType, int count, bool normalized, size_t offset );

	//-----------------------------------------------------------------------------------------------
	// Members
	const char*		m_handle; // example, "POSITION"
	RenderType		m_type;
	VKRenderType	m_vkType;
	size_t			m_memberOffset;
	bool			m_isNormalized;
	int				m_elementCount;
};

//-----------------------------------------------------------------------------------------------
struct VertexLayout // A set of vertex attributes 
{
	//-----------------------------------------------------------------------------------------------
	// Constructors
	VertexLayout( const VertexAttribute* attribs, int stride, int count );

	//-----------------------------------------------------------------------------------------------
	// Methods
	const	VertexAttribute*			GetAttribute( int index ) const { return m_attributes[index]; }

	//-----------------------------------------------------------------------------------------------
	// Members
	std::vector<const VertexAttribute*>	m_attributes;
	int									m_stride;
};


//-----------------------------------------------------------------------------------------------
struct Vertex_3DPCU
{
	//-----------------------------------------------------------------------------------------------
	// Constructors
	Vertex_3DPCU(){}
	Vertex_3DPCU( const VertexBuilder& builder );
	Vertex_3DPCU( const Vector3& position, const Rgba& color, const Vector2& UVs): m_position(position), m_color(color), m_UVs(UVs){}

	//-----------------------------------------------------------------------------------------------
	// Methods

	//-----------------------------------------------------------------------------------------------
	// Members
					Vector3			m_position;
					Rgba			m_color;
					Vector2			m_UVs;

	static	const	VertexAttribute	s_attributes[];
	static	const	VertexLayout	s_layout;
};

//-----------------------------------------------------------------------------------------------
struct VertexLit
{
	//-----------------------------------------------------------------------------------------------
	// Constructors
	VertexLit(){}
	VertexLit( const VertexBuilder& builder );
	VertexLit( const Vector3& position, const Rgba& color, const Vector2& uv, const Vector3& normal ) : m_position(position),  m_color(color), m_UVs(uv), m_normal(normal){}

	//-----------------------------------------------------------------------------------------------
	// Methods

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3			m_position;
	Rgba			m_color;
	Vector2			m_UVs;
	Vector3			m_normal = Vector3::BACK;
	Vector4			m_tangent = Vector4(Vector3::RIGHT, 1.f);

	static	const	VertexAttribute	s_attributes[];
	static	const	VertexLayout	s_layout;
};



