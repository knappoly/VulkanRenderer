#include "Engine/VulkanRenderer/Mesh/VKMeshUtils.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/VulkanRenderer/VkRenderer.hpp"

//-----------------------------------------------------------------------------------------------
// Creates a quad using mesh builder and returns a reference
//
VKMesh* CreateQuad(const Vector3& mins, const Vector3& maxs, const Rgba& color)
{
	return CreateTexturedQuad(mins, maxs, color, Vector2::ZERO, Vector2::ONE);
}

//-----------------------------------------------------------------------------------------------
// Creates a textured quad using mesh builder and returns a reference
//
VKMesh* CreateTexturedQuad(const Vector3& mins, const Vector3& maxs, const Rgba& color, const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs)
{
	MeshBuilder builder;

	builder.Begin(PRIMITIVE_TRIANGLES, true);

	builder.SetColor(color);
	builder.SetNormal(Vector3::BACK);
	builder.SetUV(texCoordsAtMins);
	builder.SetTangent(Vector3::RIGHT, 1.f);
	builder.PushVertex(mins);

	builder.SetUV(texCoordsAtMaxs.x, texCoordsAtMins.y);
	builder.PushVertex(maxs.x, mins.y);

	builder.SetUV(texCoordsAtMaxs);
	builder.PushVertex(maxs);

	builder.SetUV(texCoordsAtMins.x, texCoordsAtMaxs.y);
	builder.PushVertex(mins.x, maxs.y);

	builder.AddFaceIndices(0,1,3);
	builder.AddFaceIndices(3,1,2);

	builder.End();
	return builder.CreateMesh<VertexLit>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a 3D quad mesh and returns a reference
//
VKMesh* CreateQuad3D(const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Rgba& color /*= Rgba::WHITE */)
{
	return CreateTexturedQuad3D(position, right, up, scale, color);
}

//-----------------------------------------------------------------------------------------------
// Creates a 3D quad mesh and returns a reference
//
VKMesh* CreateTexturedQuad3D(const Vector3& position, const Vector3& right, const Vector3& up, const Vector2& scale, const Rgba& color /*= Rgba::WHITE*/, const Vector2& texCoordsAtMins /*= Vector2::ZERO*/, const Vector2& texCoordsAtMaxs /*= Vector2::ONE */)
{
	MeshBuilder builder;
	builder.Begin(PRIMITIVE_TRIANGLES, true);
	builder.SetColor(color);
	builder.AddPlane(position, right, up, scale, texCoordsAtMins, texCoordsAtMaxs);
	builder.End();
	return builder.CreateMesh<VertexLit>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a line using the mesh builder and returns a reference
//
VKMesh* CreateLine(const Vector3& start, const Vector3& end, const Rgba& color)
{
	return CreateLine(start, end, color, color);
}

//-----------------------------------------------------------------------------------------------
// Creates a line using the mesh builder and returns a reference
//
VKMesh* CreateLine(const Vector3& start, const Vector3& end, const Rgba& startVertColor /*= Rgba::WHITE*/, const Rgba& endVertColor /*= Rgba::WHITE */)
{
	MeshBuilder builder;
	builder.Begin(PRIMITIVE_LINES, false);
	builder.AddLine(start, end, startVertColor, endVertColor);
	builder.End();
	return builder.CreateMesh<VertexLit>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a cube using the mesh builder and returns a reference
//
VKMesh* CreateCube(const Vector3& center, const Vector3& size, const Rgba& color, const AABB2& uvTop, const AABB2& uvSide, const AABB2& uvBottom)
{
	MeshBuilder builder;
	builder.Begin(PRIMITIVE_TRIANGLES, true);
	builder.AddCube(center, size, color, uvTop, uvSide, uvBottom);
	builder.End();
	return builder.CreateMesh<VertexLit>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a UV sphere mesh and returns a reference to it
//
VKMesh* CreateUVSphere(const Vector3& position, float radius, uint wedges, uint slices, const Rgba& color)
{
	MeshBuilder builder;

	builder.Begin(PRIMITIVE_TRIANGLES, true);
	builder.AddSphere(position, radius, wedges, slices, color);
	builder.End();
	return builder.CreateMesh<VertexLit>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a single point mesh and returns a reference to it
//
VKMesh* CreatePoint(const Vector3& position, const Rgba& color /*= Rgba::WHITE */)
{
	MeshBuilder builder; 
	
	builder.Begin(PRIMITIVE_LINES, false);
	builder.AddPoint(position, color);
	builder.End();

	return builder.CreateMesh<Vertex_3DPCU>(VKRenderer::GetInstance());

}

//-----------------------------------------------------------------------------------------------
// Creates a basis mesh that consists of lines for each axes
//
VKMesh* CreateBasisMesh(const Matrix44& basis, float scale /*= 1.f*/)
{
	MeshBuilder builder; 

	builder.Begin(PRIMITIVE_LINES, false);
	builder.SetUV(0.f,0.f); 

	// X Axis
	builder.AddLine(basis.GetTranslation(), (basis.GetTranslation() + (basis.GetRight() * scale)), Rgba::RED, Rgba::RED);

	// Y Axis
	builder.AddLine(basis.GetTranslation(), (basis.GetTranslation() + (basis.GetUp() * scale)), Rgba::GREEN, Rgba::GREEN);

	// Z Axis
	builder.AddLine(basis.GetTranslation(), (basis.GetTranslation() + (basis.GetForward() * scale)), Rgba::BLUE, Rgba::BLUE);

	builder.End();

	return builder.CreateMesh<Vertex_3DPCU>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a text mesh and returns the reference
//
VKMesh* CreateTextMesh2D(const Vector2& position, const std::string& text, float cellHeight, const BitmapFont* font, const Rgba& tint /*= Rgba::WHITE*/, const Vector2& alignment /*= Vector2::ZERO */)
{
	MeshBuilder builder;
	float cellWidth;
	float cursorMins;
	float cursorMaxs;
	float lineMins = position.y;
	float lineMaxs = position.y + cellHeight;
	int index;

	// Compute offset based on alignment
	float offsetX = font->GetStringWidth(text, cellHeight, 1.f) * alignment.x;

	// Generating the vertices 
	builder.Begin(PRIMITIVE_TRIANGLES, true);
	builder.SetColor(tint);
	for(size_t textIndex = 0; textIndex < text.size(); ++textIndex)
	{
		cellWidth = cellHeight * font->GetGlyphAspect(text[textIndex]);
		
		cursorMins = (((float) textIndex * cellWidth) - offsetX) + position.x;
		cursorMaxs = cursorMins + cellWidth;
		AABB2 uvs = font->GetUVsForGlyph(text[textIndex]);

		builder.SetUV(uvs.mins.x, uvs.maxs.y);
		index = builder.PushVertex(cursorMins, lineMins);

		builder.SetUV(uvs.maxs.x, uvs.maxs.y);
		builder.PushVertex(cursorMaxs, lineMins);

		builder.SetUV(uvs.maxs.x, uvs.mins.y);
		builder.PushVertex(cursorMaxs, lineMaxs);

		builder.SetUV(uvs.mins);
		builder.PushVertex(cursorMins, lineMaxs);

		builder.AddQuadIndices(index, index + 1, index + 2, index + 3);
	}

	builder.End();
	return builder.CreateMesh<Vertex_3DPCU>(VKRenderer::GetInstance());
}

//-----------------------------------------------------------------------------------------------
// Creates a grid mesh with each cell as a 1x1 box and returns a reference 
//
VKMesh* CreateGridMesh(const Vector3& right, const Vector3& up, int uniformSize, const Rgba& color /*= Rgba::WHITE */)
{
	MeshBuilder builder;
	builder.Begin(PRIMITIVE_LINES, false);
	
	Vector3 drawMins;
	float offset = -uniformSize * 0.5f;

	drawMins = (right * offset) + (up * offset);

	for(int rowIndex = 0; rowIndex <= uniformSize; ++rowIndex)
	{
		builder.AddLine(drawMins, drawMins + (right * (float)uniformSize), color, color);
		drawMins += up;
	}

	drawMins = (right * offset) + (up * offset);
	for(int colIndex = 0; colIndex <= uniformSize; ++colIndex)
	{
		builder.AddLine(drawMins, drawMins + (up * (float)uniformSize), color, color);
		drawMins += right;
	}

	builder.End();
	return builder.CreateMesh<Vertex_3DPCU>(VKRenderer::GetInstance());
}
