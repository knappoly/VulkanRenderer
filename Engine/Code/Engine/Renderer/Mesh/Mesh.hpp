#pragma once
#include "Engine/Structures/DrawInstruction.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint;
enum DrawPrimitiveType;
struct Vertex_3DPCU;
class VertexBuffer;
class IndexBuffer;
struct VertexLayout;
class MeshBuilder;
struct VertexLit;

//-----------------------------------------------------------------------------------------------
class Mesh
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Mesh();
	~Mesh();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			void			SetVertices( uint count, const void* vertices, const VertexLayout& layout );
			void			SetIndices( uint count, const uint* indices );
			void			SetDrawInstructions( DrawPrimitiveType type, bool useIndices, size_t startIndex, uint elementCount );
			void			SetDrawInstructions( const DrawInstruction& instructions );
	const	VertexLayout*	GetLayout() const { return m_layout; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			template<typename VERTTYPE>
			void			FromBuilder( const MeshBuilder& builder );

			template<typename VERTTYPE>
			void			FromFile( const char* path );
	
	//-----------------------------------------------------------------------------------------------
	// Members
			VertexBuffer*	m_vbo = nullptr;
			IndexBuffer*	m_ibo = nullptr;
	const	VertexLayout*	m_layout = nullptr;
			DrawInstruction m_drawInstruction;
};

template void Mesh::FromBuilder<VertexLit>( const MeshBuilder& builder );
template void Mesh::FromBuilder<Vertex_3DPCU>( const MeshBuilder& builder );

template void Mesh::FromFile<VertexLit>( const char* path );
template void Mesh::FromFile<Vertex_3DPCU>( const char* path );

