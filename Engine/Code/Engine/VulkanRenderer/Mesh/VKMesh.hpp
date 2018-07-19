#pragma once
#include "Engine/Structures/DrawInstruction.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;
struct Vertex_3DPCU;
struct VertexLit;
class VKVertexBuffer;
class VKIndexBuffer;
struct VertexLayout;
class MeshBuilder;

//-----------------------------------------------------------------------------------------------
class VKMesh
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKMesh( VKRenderer* renderer );
	~VKMesh();
	
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
			VKRenderer*		m_renderer = nullptr;
			VKVertexBuffer*	m_vbo = nullptr;
			VKIndexBuffer*	m_ibo = nullptr;
	const	VertexLayout*	m_layout = nullptr;
			DrawInstruction m_drawInstruction;
};

template void VKMesh::FromBuilder<VertexLit>( const MeshBuilder& builder );
template void VKMesh::FromBuilder<Vertex_3DPCU>( const MeshBuilder& builder );

template void VKMesh::FromFile<VertexLit>( const char* path );
template void VKMesh::FromFile<Vertex_3DPCU>( const char* path );
