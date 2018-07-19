#pragma once
#include "Engine/Core/Vertex.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Math/AABB2.hpp"
#include <functional>
#include "Engine/VulkanRenderer/Mesh/VKMesh.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint;
enum DrawPrimitiveType;
class VKRenderer;

//-----------------------------------------------------------------------------------------------
// Function callbacks
typedef std::function<Vector3(float,float)> SurfaceCb;

//-----------------------------------------------------------------------------------------------
class MeshBuilder
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	MeshBuilder(){}
	~MeshBuilder(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	uint			GetVertexCount() const { return (uint) m_vertices.size(); }
	uint			GetIndicesCount() const { return (uint) m_indices.size(); }
	uint			GetIndex( int index ) const { return m_indices[index]; }
	VertexBuilder	GetVertex( int index ) const { return m_vertices[index]; }
	DrawInstruction GetDrawInstructions() const { return m_drawInstruction; }

	void			SetColor( const Rgba& color );

	void			SetUV( const Vector2& uv );
	void			SetUV( float u, float v );

	void			SetNormal( const Vector3& normal );
	void			SetNormal( float x, float y, float z ) { SetNormal(Vector3(x,y,z)); }

	void			SetTangent( const Vector4& tangent );
	void			SetTangent( const Vector3& dir, float fsign ) { SetTangent(Vector4(dir, fsign)); }
	void			SetTangent( float x, float y, float z, float fsign ) { SetTangent(Vector4(x,y,z,fsign)); }

	uint			PushVertex( const Vector3& vertex );
	uint			PushVertex( const float x, const float y, const float z );
	uint			PushVertex( const float x, const float y );

	//-----------------------------------------------------------------------------------------------
	// Methods
	void			Begin( DrawPrimitiveType primitive, bool useIndices );
	void			End();
	void			Flush();

	// Builder Helpers
	void			LoadFromFile( const char* path );
	void			AddFaceIndices( uint index1, uint index2, uint index3 );
	void			AddQuadIndices( uint blIdx, uint brIdx, uint trIdx, uint tlIdx );

	void			AddPlane( const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Vector2& texCoordsAtMins = Vector2::ZERO, const Vector2& texCoordsAtMaxs = Vector2::ONE );
	void			AddCube( const Vector3& center, const Vector3& size, const Rgba& color = Rgba::WHITE, const AABB2& uvTop = AABB2::ZERO_TO_ONE, 
							const AABB2& uvSide = AABB2::ZERO_TO_ONE, const AABB2& uvBottom = AABB2::ZERO_TO_ONE );
	void			AddSphere( const Vector3& position, float radius, uint wedges, uint slices, const Rgba& color );
	void			AddPoint( const Vector3& position, const Rgba& color = Rgba::WHITE );
	void			AddLine( const Vector3& start, const Vector3& end, const Rgba& startVertColor = Rgba::WHITE, const Rgba& endVertColor = Rgba::WHITE );
	void			AddSurfacePatch( SurfaceCb callback, float umin, float umax, uint wedges, float vmin, float vmax, uint slices, const Rgba& color = Rgba::WHITE );

	Vector3			CalculateNormal( const Vector3& tangentStart, const Vector3& tangentEnd, const Vector3& bitangentStart, const Vector3& bitangentEnd );
	Vector3			CalculateNormal ( const Vector3& tangent, const Vector3& bitangent ) ;

	template<typename VERTTYPE>
	Mesh*			CreateMesh()
	{
		Mesh* mesh = new Mesh();
		mesh->FromBuilder<VERTTYPE>(*this);
		return mesh;
	}

	template<typename VERTTYPE>
	VKMesh*			CreateMesh( VKRenderer* renderer )
	{
		VKMesh* mesh = new VKMesh(renderer);
		mesh->FromBuilder<VERTTYPE>(*this);
		return mesh;
	}
	
	
	//-----------------------------------------------------------------------------------------------
	// Members
	VertexBuilder				m_stamp;
	std::vector<VertexBuilder>	m_vertices;
	std::vector<uint>			m_indices;
	DrawInstruction				m_drawInstruction;
	int							m_polygonCount = 0;
	int							m_triCount = 0;
};

