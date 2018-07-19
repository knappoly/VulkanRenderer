#include "Engine/Renderer/ForwardRenderPath.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/RenderScene.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Renderable.hpp"
#include "Engine/Renderer/DrawCall.hpp"
#include "Engine/Renderer/Lights/Light.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Material.hpp"
#include "Engine/Renderer/Sampler.hpp"
//#include "Engine/Profiler/Profiler.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constuctor
//
ForwardRenderPath::ForwardRenderPath()
{
	m_shadowCamera = new Camera();
	Texture* depthTarget = Renderer::GetInstance()->CreateDepthStencilTarget(2048, 2048);
	depthTarget->SetSampler(Sampler::GetShadowSampler());
	Texture* colorTarget = Renderer::GetInstance()->CreateRenderTarget(2048, 2048);
	m_shadowCamera->SetDepthTarget( depthTarget );
	m_shadowCamera->SetColorTarget( colorTarget );
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
ForwardRenderPath::~ForwardRenderPath()
{
	delete m_shadowCamera;
	m_shadowCamera = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Renders the scene 
//
void ForwardRenderPath::Render(RenderScene* scene)
{
	//PROFILE_LOG_SCOPE_FUNCTION();

#ifdef DEBUG_RENDER_LIGHTS 
	for(Light* light: scene->m_lights)
	{
		light->DebugRender();
	}
#endif

	// Independent pre-renders
	for(PrerenderIndieCB cb : scene->m_indiePrerenders)
	{
		cb();
	}

	// Light pre-render for shadow casting lights
	for(Light* light : scene->m_lights)
	{
		if(light->IsShadowCasting())
		{
			RenderShadowObjectForLight(light, scene);
		}
	}

	// Render the scene for every camera 
	for(Camera* cam : scene->m_cameras)
	{
		RenderSceneForCamera(cam, scene);
	}
}

//-----------------------------------------------------------------------------------------------
// Renders the scene for one camera
//
void ForwardRenderPath::RenderSceneForCamera(Camera* cam, RenderScene* scene)
{
	Renderer* rend = Renderer::GetInstance();
	rend->SetCamera(cam);
	
	if(cam->IsSkyBoxValid())
	{
		rend->BindCubemap(cam->GetSkyBoxTexture());
		rend->SetShader(Shader::CreateOrGetResource("Data/Shaders/skybox.shader"));
		rend->SetDepthTestMode(COMPARE_ALWAYS, false);
		rend->SetCullMode(CULLMODE_NONE);
		rend->SetDefaultMaterial();
		rend->DrawMesh(rend->CreateOrGetMesh("Cube"));
		rend->SetDepthTestMode(COMPARE_ALWAYS, true);
		rend->ResetDefaultMaterial();
	}

	std::vector<DrawCall> drawCalls;

	for(PrerenderCB preRender : scene->m_prerenders)
	{
		preRender(cam);
	}

	for(Renderable* renderable : scene->m_renderables)
	{
		std::vector<Light*> lights;
		if(renderable->IsLit())
		{
			lights = scene->GetMostContributingLights(renderable->GetPosition());
		}

		if(renderable->IsOpaque())
		{
			renderable->GetEditableMaterial()->SetTexture(TEXTURE_SLOT_SHADOWMAP, m_shadowCamera->GetDepthTarget());
		}

		DrawCall dc;
		dc.m_mesh = renderable->m_mesh;
		dc.m_material = renderable->GetMaterial();
		dc.m_lights = lights;
		dc.m_model = renderable->GetModelMatrix();
		dc.m_transform = renderable->m_watchTransform;
		drawCalls.push_back(dc);
	}

	SortDrawsBySortOrder(drawCalls);
	SortDrawsByRenderQueue(drawCalls);
	SortDrawsByCameraDistance(drawCalls, cam);
	for(DrawCall dc : drawCalls)
	{
		rend->SetLightBuffer(dc.m_lights);
		rend->SetMaterial(dc.m_material);
		rend->DrawMesh(dc.m_mesh, dc.m_model);
	}
}

//-----------------------------------------------------------------------------------------------
// Renders the shadow casting objects for the given light
//
void ForwardRenderPath::RenderShadowObjectForLight(Light* light, RenderScene* scene)
{
	m_shadowCamera->SetOrtho(-128.f, 128.f, -128.f, 128.f, 0.f, 100.f);
	
	Vector3 worldPos = light->GetWorldPosition();

	m_shadowCamera->LookAt(worldPos, worldPos + light->m_transform->GetForward());

	Renderer* rend = Renderer::GetInstance();
	rend->SetCamera(m_shadowCamera);
	rend->ClearDepth();
	rend->ClearScreen(Rgba::BLACK);
	Matrix44 VP = m_shadowCamera->m_projMatrix * m_shadowCamera->m_viewMatrix;
	light->SetViewProjection(VP);

	rend->SetMaterial(rend->CreateOrGetMaterial("Data/Materials/shadow.mat"));
	for(Renderable* renderable : scene->m_renderables)
	{
		if(renderable->IsOpaque())
		{
			rend->DrawMesh(renderable->GetMesh(), renderable->GetModelMatrix());
		}
	}

	rend->ResetDefaultMaterial();
}
//-----------------------------------------------------------------------------------------------
// Sort the draw calls by sort order
//
void ForwardRenderPath::SortDrawsBySortOrder(std::vector<DrawCall>& drawCalls)
{
	for(size_t indexI = 0; indexI < drawCalls.size(); ++indexI)
	{
		for(size_t indexJ = indexI + 1; indexJ < drawCalls.size(); ++indexJ)
		{
			if(drawCalls[indexJ].GetSortOrder() < drawCalls[indexI].GetSortOrder())
			{
				std::swap(drawCalls[indexI], drawCalls[indexJ]);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Sort the draws by render queue
//
void ForwardRenderPath::SortDrawsByRenderQueue(std::vector<DrawCall>& drawCalls)
{
	for(size_t indexI = 0; indexI < drawCalls.size(); ++indexI)
	{
		for(size_t indexJ = indexI + 1; indexJ < drawCalls.size(); ++indexJ)
		{
			int sortOrderI = drawCalls[indexI].GetSortOrder();
			int sortOrderJ = drawCalls[indexJ].GetSortOrder();
			if( sortOrderJ > sortOrderI)
			{
				break;
			}

			if((sortOrderI == sortOrderJ) && (drawCalls[indexJ].GetRenderQueue() < drawCalls[indexI].GetRenderQueue()))
			{
				std::swap(drawCalls[indexI], drawCalls[indexJ]);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Sorts draw calls by camera depth
//
void ForwardRenderPath::SortDrawsByCameraDistance(std::vector<DrawCall>& drawCalls, Camera* cam)
{
	std::vector<float> distances;
	for(DrawCall dc : drawCalls)
	{
		Vector3 pos = dc.m_model.GetTranslation();
		Vector3 cameraPos = cam->m_transform.GetWorldPosition();

		float distance = (cameraPos - pos).GetLength();
		distances.push_back(distance);
	}
	
	for(size_t indexI = 0; indexI < drawCalls.size(); ++indexI)
	{
		for(size_t indexJ = indexI + 1; indexJ < drawCalls.size(); ++indexJ)
		{
			int sortOrderI = drawCalls[indexI].GetSortOrder();
			int sortOrderJ = drawCalls[indexJ].GetSortOrder();
			if( sortOrderJ > sortOrderI)
			{
				break;
			}

			int renderQueueI = drawCalls[indexI].GetRenderQueue();
			int renderQueueJ = drawCalls[indexJ].GetRenderQueue();
			if( renderQueueJ > renderQueueI )
			{
				break; 
			}
			if((sortOrderI == sortOrderJ) && (renderQueueI == renderQueueJ) && (distances[indexJ] < distances[indexI]))
			{
				std::swap(drawCalls[indexI], drawCalls[indexJ]);
				std::swap(distances[indexI], distances[indexJ]);
			}
		}
	}
}
