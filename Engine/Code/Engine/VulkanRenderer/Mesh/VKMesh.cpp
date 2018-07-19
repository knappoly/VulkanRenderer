#include "Engine/VulkanRenderer/Mesh/VKMesh.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/Buffers/VKVertexBuffer.hpp"
#include "Engine/VulkanRenderer/Buffers/VKIndexBuffer.hpp"
#include "Engine/Core/Vertex.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Standard Includes

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKMesh::VKMesh(VKRenderer* renderer)
{
	m_renderer = renderer;
	m_vbo = new VKVertexBuffer(renderer);
	m_ibo = new VKIndexBuffer(renderer);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKMesh::~VKMesh()
{
	delete m_ibo;
	m_ibo = nullptr;

	delete m_vbo;
	m_vbo = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Copies the vertex data into the vertex buffer
//
void VKMesh::SetVertices(uint count, const void* vertices, const VertexLayout& layout)
{
	m_vbo->SetStride(layout.m_stride);
	m_vbo->SetCount(count);

	m_vbo->CopyToGPU( count * layout.m_stride, vertices ); 
	m_layout = &layout;
}

//-----------------------------------------------------------------------------------------------
// Copies the index data into the index buffer
//
void VKMesh::SetIndices(uint count, const uint* indices)
{
	uint stride = sizeof(uint);
	m_ibo->SetStride(stride);
	m_ibo->SetCount(count);

	m_ibo->CopyToGPU(count * stride, indices);
}

//-----------------------------------------------------------------------------------------------
// Sets the draw instructions
//
void VKMesh::SetDrawInstructions(DrawPrimitiveType type, bool useIndices, size_t startIndex, uint elementCount)
{
	m_drawInstruction.m_drawType = type;
	m_drawInstruction.m_useIndices = useIndices;
	m_drawInstruction.m_startIndex = startIndex;
	m_drawInstruction.m_elementCount = elementCount;
}

//-----------------------------------------------------------------------------------------------
// Sets the draw instructions
//
void VKMesh::SetDrawInstructions(const DrawInstruction& instructions)
{
	SetDrawInstructions(instructions.m_drawType, instructions.m_useIndices, instructions.m_startIndex, instructions.m_elementCount);
}

//-----------------------------------------------------------------------------------------------
// Creates this mesh from a builder object
//
template<typename VERTTYPE>
void VKMesh::FromBuilder(const MeshBuilder& builder)
{
	uint vcount = builder.GetVertexCount(); 
	VERTTYPE* temp = (VERTTYPE*)malloc( sizeof(VERTTYPE) * vcount ); 

	for (uint index = 0; index < vcount; ++index) 
	{
		// copy each vertex
		temp[index] = VERTTYPE( builder.GetVertex(index) ); 
	}

	SetVertices(vcount, temp, VERTTYPE::s_layout);

	// Copy indices
	if(builder.GetDrawInstructions().m_useIndices)
	{
		SetIndices(builder.GetIndicesCount(), builder.m_indices.data());
	}

	SetDrawInstructions(builder.GetDrawInstructions());

	// Cleanup
	free(temp);
}

//-----------------------------------------------------------------------------------------------
// Creates this mesh from a file (Supported types: .obj)
//
template<typename VERTTYPE>
void VKMesh::FromFile(const char* path)
{
	MeshBuilder builder;
	builder.LoadFromFile(path);

	FromBuilder<VERTTYPE>(builder);
}