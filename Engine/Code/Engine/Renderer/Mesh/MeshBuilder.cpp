#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/File/File.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
static const char* OBJPrefixes[] = { "v", "vn", "vt", "f" };

//-----------------------------------------------------------------------------------------------
enum OBJLinePrefixes
{
	PREFIX_V,
	PREFIX_VN,
	PREFIX_VT,
	PREFIX_F
};

//-----------------------------------------------------------------------------------------------
// Sets the color on the stamp
//
void MeshBuilder::SetColor(const Rgba& color)
{
	m_stamp.m_color = color;
}

//-----------------------------------------------------------------------------------------------
// Sets the uvs on the stamp
//
void MeshBuilder::SetUV(const Vector2& uv)
{
	m_stamp.m_UV = uv;
}

//-----------------------------------------------------------------------------------------------
// Sets the uvs on the stamp
//
void MeshBuilder::SetUV(float u, float v)
{
	SetUV(Vector2(u,v));
}

//-----------------------------------------------------------------------------------------------
// Sets the normal on the stamp
//
void MeshBuilder::SetNormal(const Vector3& normal)
{
	m_stamp.m_normal = normal;
}

//-----------------------------------------------------------------------------------------------
// Sets the tangent on the stamp
//
void MeshBuilder::SetTangent(const Vector4& tangent)
{
	m_stamp.m_tangent = tangent;
}

//-----------------------------------------------------------------------------------------------
// Pushes the vertex into the list
//
uint MeshBuilder::PushVertex(const Vector3& vertex)
{
	m_stamp.m_position = vertex;
	m_vertices.push_back(m_stamp);

	return (uint) (m_vertices.size() - 1); // Returns the index of the currently pushed vertex
}

//-----------------------------------------------------------------------------------------------
// Pushes the vertex into the list of vertices
//
uint MeshBuilder::PushVertex(float x, float y,  float z)
{
	return PushVertex(Vector3(x,y,z));
}

//-----------------------------------------------------------------------------------------------
// Pushes the vertex into the list of vertices
//
uint MeshBuilder::PushVertex(const float x, const float y)
{
	return PushVertex(x, y, 0.f);
}

//-----------------------------------------------------------------------------------------------
// Adds a face to the builder using indices
//
void MeshBuilder::AddFaceIndices(uint index1, uint index2, uint index3)
{
	m_indices.push_back(index1);
	m_indices.push_back(index2);
	m_indices.push_back(index3);
}

//-----------------------------------------------------------------------------------------------
// Adds a quad face to the builder using indices
//
void MeshBuilder::AddQuadIndices(uint blIdx, uint brIdx, uint trIdx, uint tlIdx)
{
	m_indices.push_back(blIdx);
	m_indices.push_back(brIdx);
	m_indices.push_back(tlIdx);
	
	m_indices.push_back(tlIdx);
	m_indices.push_back(brIdx);
	m_indices.push_back(trIdx);
}

//-----------------------------------------------------------------------------------------------
// Adds a plane to the builder using indices
//
void MeshBuilder::AddPlane(const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Vector2& texCoordsAtMins /*= Vector2::ZERO*/, const Vector2& texCoordsAtMaxs /*= Vector2::ONE */)
{
	Vector3 points[4] = 
	{
		Vector3(position),
		Vector3(position + (right * scale.x)),
		Vector3(position + (right * scale.x) + (up * scale.y)),
		Vector3(position + (up * scale.y))
	};

	SetNormal(Vector3::BACK);
	SetUV(texCoordsAtMins);
	int index = PushVertex(points[0]);

	SetUV(texCoordsAtMaxs.x, texCoordsAtMins.y);
	PushVertex(points[1]);

	SetUV(texCoordsAtMaxs);
	PushVertex(points[2]);

	SetUV(texCoordsAtMins.x, texCoordsAtMaxs.y);
	PushVertex(points[3]);

	AddQuadIndices(index + 0, index + 1, index + 2, index + 3);
}

//-----------------------------------------------------------------------------------------------
// Adds a cube to the builder using indices
//
void MeshBuilder::AddCube(const Vector3& center, const Vector3& size, const Rgba& color /*= Rgba::WHITE*/, const AABB2& uvTop /*= AABB2::ZERO_TO_ONE*/, const AABB2& uvSide /*= AABB2::ZERO_TO_ONE*/, const AABB2& uvBottom /*= AABB2::ZERO_TO_ONE */)
{
	float sizeHalfX = size.x * 0.5f;
	float sizeHalfY = size.y * 0.5f;
	float sizeHalfZ = size.z * 0.5f;

	Vector3 points[] = {
		Vector3(center.x - sizeHalfX, center.y - sizeHalfY, center.z - sizeHalfZ),
		Vector3(center.x + sizeHalfX, center.y - sizeHalfY, center.z - sizeHalfZ),
		Vector3(center.x + sizeHalfX, center.y + sizeHalfY, center.z - sizeHalfZ), 
		Vector3(center.x - sizeHalfX, center.y + sizeHalfY, center.z - sizeHalfZ),
		Vector3(center.x - sizeHalfX, center.y - sizeHalfY, center.z + sizeHalfZ),
		Vector3(center.x + sizeHalfX, center.y - sizeHalfY, center.z + sizeHalfZ),
		Vector3(center.x + sizeHalfX, center.y + sizeHalfY, center.z + sizeHalfZ), 
		Vector3(center.x - sizeHalfX, center.y + sizeHalfY, center.z + sizeHalfZ)
	};

	uint index;

	SetColor(color);
	SetUV(uvSide.mins); // Back Face : That is the one that looks at us initially
	SetNormal(Vector3::BACK);
	SetTangent(Vector3::RIGHT, 1.f);
	index = PushVertex(points[0]);
	SetUV(uvSide.maxs.x, uvSide.mins.y);
	PushVertex(points[1]);
	SetUV(uvSide.maxs);
	PushVertex(points[2]);
	SetUV(uvSide.mins.x, uvSide.maxs.y);
	PushVertex(points[3]);
	AddFaceIndices(index + 0, index + 1, index + 3);
	AddFaceIndices(index + 3, index + 1, index + 2);

	SetUV(uvSide.mins); // Right Face
	SetNormal(Vector3::RIGHT);
	SetTangent(Vector3::FORWARD, 1.f);
	index = PushVertex(points[1]);
	SetUV(uvSide.maxs.x, uvSide.mins.y);
	PushVertex(points[5]);
	SetUV(uvSide.maxs);
	PushVertex(points[6]);
	SetUV(uvSide.mins.x, uvSide.maxs.y);
	PushVertex(points[2]);
	AddFaceIndices(index + 0, index + 1, index + 3);
	AddFaceIndices(index + 3, index + 1, index + 2);

	SetUV(uvSide.mins); // Front Face:  Face that points away from the initial position
	SetNormal(Vector3::FORWARD);
	SetTangent(Vector3::LEFT, 1.f);
	index = PushVertex(points[5]);
	SetUV(uvSide.maxs.x, uvSide.mins.y);
	PushVertex(points[4]);
	SetUV(uvSide.maxs);
	PushVertex(points[7]);
	SetUV(uvSide.mins.x, uvSide.maxs.y);
	PushVertex(points[6]);
	AddFaceIndices(index + 0, index + 1, index + 3);
	AddFaceIndices(index + 3, index + 1, index + 2);

	SetUV(uvSide.mins); // Left Face
	SetNormal(Vector3::LEFT);
	SetTangent(Vector3::BACK, 1.f);
	index = PushVertex(points[4]);
	SetUV(uvSide.maxs.x, uvSide.mins.x);
	PushVertex(points[0]);
	SetUV(uvSide.maxs);
	PushVertex(points[3]);
	SetUV(uvSide.mins.x, uvSide.maxs.y);
	PushVertex(points[7]);
	AddFaceIndices(index + 0, index + 1, index + 3);
	AddFaceIndices(index + 3, index + 1, index + 2);

	SetUV(uvTop.mins); // Top Face
	SetNormal(Vector3::UP);
	SetTangent(Vector3::RIGHT, 1.f);
	index = PushVertex(points[3]);
	SetUV(uvTop.maxs.x, uvTop.mins.y);
	PushVertex(points[2]);
	SetUV(uvTop.maxs);
	PushVertex(points[6]);
	SetUV(uvTop.mins.x, uvTop.maxs.y);
	PushVertex(points[7]);
	AddFaceIndices(index + 0, index + 1, index + 3);
	AddFaceIndices(index + 3, index + 1, index + 2);

	SetUV(uvBottom.mins); // Bottom Face
	SetNormal(Vector3::DOWN);
	SetTangent(Vector3::RIGHT, 1.f);
	index = PushVertex(points[4]);
	SetUV(uvBottom.maxs.x, uvBottom.mins.y);
	PushVertex(points[5]);
	SetUV(uvBottom.maxs);
	PushVertex(points[1]);
	SetUV(uvBottom.mins.x, uvBottom.maxs.y);
	PushVertex(points[0]);
	AddFaceIndices(index + 0, index + 1, index + 3);
	AddFaceIndices(index + 3, index + 1, index + 2);
}

//-----------------------------------------------------------------------------------------------
// Adds a cube to the builder using indices
//
void MeshBuilder::AddSphere(const Vector3& position, float radius, uint wedges, uint slices, const Rgba& color)
{
	for(uint sliceIndex = 0; sliceIndex <= slices; ++sliceIndex)
	{
		float v = (float) sliceIndex * 1.f / (float) (slices - 1);
		float azimuth = RangeMapFloat(v, 0.f, 1.f, -90.f, 90.f);

		for(uint wedgeIndex = 0; wedgeIndex <= wedges; ++wedgeIndex)
		{
			float u = (float) wedgeIndex * 1.f / (float) (wedges - 1);
			float rotation = RangeMapFloat(u, 0.f, 1.f, 0.f, 360.f);

			SetColor(color);
			SetUV(u,v);
			Vector3 pos = position + SphericalToCartesian(radius, rotation, azimuth);
			Vector3 normal = pos - position;
			SetNormal(normal.GetNormalized());
			float tanX = - 1.f * CosDegrees(azimuth) * SinDegrees(rotation) * radius;
			float tanZ = CosDegrees(azimuth) * CosDegrees(rotation) * radius;
			SetTangent(tanX, 0.f, tanZ, 1.f);
			PushVertex(pos);
		}
	}

	for(uint sliceIndex = 0; sliceIndex < slices; ++sliceIndex)
	{
		for(uint wedgeIndex = 0; wedgeIndex < wedges; ++wedgeIndex)
		{
			uint blIndex = ((wedges + 1) *  sliceIndex) + wedgeIndex;
			uint tlIndex = blIndex + (wedges + 1);
			uint brIndex = blIndex + 1;
			uint trIndex = tlIndex + 1;

			AddQuadIndices(blIndex, brIndex, trIndex, tlIndex);
		}
	}

}

//-----------------------------------------------------------------------------------------------
// Adds a point represented as a cross to the builder using indices
//
void MeshBuilder::AddPoint(const Vector3& position, const Rgba& color /* = Rgba::WHITE */)
{
	SetColor(color);
	SetUV(0.f,0.f); 

	PushVertex(position + (Vector3::RIGHT * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3::RIGHT * POINT_RENDER_SCALE));

	PushVertex(position + (Vector3::UP * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3::UP * POINT_RENDER_SCALE));

	PushVertex(position + (Vector3::FORWARD * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3::FORWARD * POINT_RENDER_SCALE));

	PushVertex(position + (Vector3(0.5f, 0.5f) * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3(0.5f, 0.5f) * POINT_RENDER_SCALE));

	PushVertex(position + (Vector3(-0.5f, 0.5f) * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3(-0.5f, 0.5f) * POINT_RENDER_SCALE));

	PushVertex(position + (Vector3(0.f, 0.5f, 0.5f) * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3(0.f, 0.5f, 0.5f) * POINT_RENDER_SCALE));

	PushVertex(position + (Vector3(0.f, 0.5f, -0.5f) * -POINT_RENDER_SCALE));
	PushVertex(position + (Vector3(0.f, 0.5f, -0.5f) * POINT_RENDER_SCALE));
}

//-----------------------------------------------------------------------------------------------
// Adds a line to the set of vertices 
//
void MeshBuilder::AddLine(const Vector3& start, const Vector3& end, const Rgba& startVertColor /*= Rgba::WHITE*/, const Rgba& endVertColor /*= Rgba::WHITE */)
{
	SetColor(startVertColor);
	SetUV(0.f,0.f); 
	PushVertex(start);

	SetColor(endVertColor);
	SetUV(0.f,0.f);
	PushVertex(end);
}

//-----------------------------------------------------------------------------------------------
// Adds a surface patch to the builder
//
void MeshBuilder::AddSurfacePatch( SurfaceCb callback, float umin, float umax, uint wedges, float vmin, float vmax, uint slices, const Rgba& color /*= Rgba::WHITE */)
{
	float sliceFraction = (umax - umin) / (float) (slices);
	float wedgeFraction = (vmax - vmin) / (float) (wedges);

	for(uint sliceIndex = 0; sliceIndex <= slices; ++sliceIndex)
	{
		float v = vmin + (float) sliceIndex * sliceFraction;
		for(uint wedgeIndex = 0; wedgeIndex <= wedges; ++wedgeIndex)
		{
			float u = umin + (float) wedgeIndex * wedgeFraction;
			SetUV(u, v);
			SetColor(color);
			Vector3 pos = callback(u,v);

			Vector3 nextPos = callback ( u + wedgeFraction, v );
			Vector3 tangent = (nextPos - pos).GetNormalized();

			nextPos = callback ( u, v + sliceFraction );
			Vector3 biTangent = (nextPos - pos).GetNormalized();
			SetTangent(tangent, 1.f);
			SetNormal(CalculateNormal(tangent, biTangent));
			PushVertex(pos);
		}
	}

	for(uint sliceIndex = 0; sliceIndex < slices; ++sliceIndex)
	{
		for(uint wedgeIndex = 0; wedgeIndex < wedges; ++wedgeIndex)
		{
			uint blIndex = ((wedges + 1) *  sliceIndex) + wedgeIndex;
			uint tlIndex = blIndex + (wedges + 1);
			uint brIndex = blIndex + 1;
			uint trIndex = tlIndex + 1;

			AddQuadIndices(blIndex, brIndex, trIndex, tlIndex);
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Calculates normals for the face
//
Vector3 MeshBuilder::CalculateNormal(const Vector3& tangentStart, const Vector3& tangentEnd, const Vector3& bitangentStart, const Vector3& bitangentEnd)
{
	Vector3 tangent = tangentEnd - tangentStart;
	Vector3 bitangent = bitangentEnd - bitangentStart;
	return CalculateNormal(tangent, bitangent);
}

//-----------------------------------------------------------------------------------------------
// Calculates normals for the face
//
Vector3 MeshBuilder::CalculateNormal(const Vector3& tangent, const Vector3& bitangent)
{
	Vector3 normal = CrossProduct(bitangent, tangent);
	normal.Normalize();

	return normal;
}

//-----------------------------------------------------------------------------------------------
// Begins draw 
//
void MeshBuilder::Begin(DrawPrimitiveType primitive, bool useIndices)
{
	m_drawInstruction.m_drawType = primitive; 
	m_drawInstruction.m_useIndices = useIndices; 

	if (useIndices) 
	{
		m_drawInstruction.m_startIndex = (uint) m_indices.size(); 
	} 
	
	else 
	{
		m_drawInstruction.m_startIndex = (uint) m_vertices.size(); 
	}
}

//-----------------------------------------------------------------------------------------------
// Ends a draw
//
void MeshBuilder::End()
{
	size_t endIndex;;
	if (m_drawInstruction.m_useIndices) 
	{
		endIndex = m_indices.size(); 
	}
	
	else 
	{
		endIndex = m_vertices.size(); 
	}

	m_drawInstruction.m_elementCount = (uint) (endIndex - m_drawInstruction.m_startIndex); 
}

//-----------------------------------------------------------------------------------------------
// Flushes all the data
//
void MeshBuilder::Flush()
{
	m_vertices.clear();
	m_indices.clear();
	m_polygonCount = 0;
	m_triCount = 0;
	m_drawInstruction = DrawInstruction();
}

//-----------------------------------------------------------------------------------------------
// Loads an obj file
//
void MeshBuilder::LoadFromFile(const char* path)
{
	std::string objSrc = (char*) FileReadToNewBuffer(path);
	
	// Temporary containers
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;

	StringTokenizer fileTokenizer(objSrc, "\n");
	fileTokenizer.Tokenize();
	fileTokenizer.TrimEmpty();
	Strings lineTokens = fileTokenizer.GetTokens();

	Begin(PRIMITIVE_TRIANGLES, true);
	for(size_t lineIndex = 0; lineIndex < fileTokenizer.GetTokenCount(); ++lineIndex)
	{
		StringTokenizer lineTokenizer(lineTokens[lineIndex], " "); // Tokenizes with white spaces
		lineTokenizer.Tokenize();
		lineTokenizer.TrimEmpty();
		Strings tokens = lineTokenizer.GetTokens();
		
		if(tokens[0] == OBJPrefixes[PREFIX_V])
		{
			Vector3 vertex;
			vertex.SetFromText((tokens[1] + "," + tokens[2] + "," + tokens[3]).c_str());
			vertex.x = -vertex.x; // X basis is flipped in OBJ file(Right handed system)
			vertices.push_back(vertex);
		}
		else if(tokens[0] == OBJPrefixes[PREFIX_VN])
		{
			Vector3 normal;
			normal.SetFromText((tokens[1] + "," + tokens[2] + "," + tokens[3]).c_str());
			normal.x = -normal.x; // X basis is flipped in OBJ file(Right handed system)
			normals.push_back(normal);
		}
		else if(tokens[0] == OBJPrefixes[PREFIX_VT])
		{
			Vector2 uv;
			uv.SetFromText((tokens[1] + "," + tokens[2]).c_str());
			uvs.push_back(uv);
		}
		else if(tokens[0] == OBJPrefixes[PREFIX_F])
		{
			uint vertIndex = (uint) m_vertices.size(); // Not -1 coz the vertex is yet to be added
			size_t tokenCount = tokens.size() - 1; // -1 to not consider prefix

			// Format: POS/TEX/NORMAL -> v/vt/vn
			for(size_t index = 1; index < tokens.size(); ++index)
			{
				StringTokenizer indexTokenizer(tokens[index], "/");
				indexTokenizer.Tokenize();
				
				Strings indices = indexTokenizer.GetTokens();
				int posIndex = stoi(indices[0]) - 1;
				int uvIndex = stoi(indices[1]) - 1;
				int normalIndex = stoi(indices[2]) - 1;

				SetUV(uvs[uvIndex]);
				SetNormal(normals[normalIndex]);
				PushVertex(vertices[posIndex]);
			}

			if( tokenCount == 3) // Triangle face
			{
				m_triCount++;
				AddFaceIndices(vertIndex, vertIndex + 1, vertIndex + 2);
			}
			else if(tokenCount == 4) // Quad face
			{
				m_polygonCount++;
				AddQuadIndices(vertIndex, vertIndex + 1, vertIndex + 2, vertIndex + 3);
			}
		}
	}
	End();
}

