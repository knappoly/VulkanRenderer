#include "Engine/Renderer/RenderScene.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Renderable.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/Lights/Light.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Renderer.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Gets the list of the most contributing lights 
//
std::vector<Light*> RenderScene::GetMostContributingLights(const Vector3& position)
{
	if(m_lights.size() <= MAX_LIGHTS)
	{
		return m_lights;
	}

	std::vector<float> lightPowers;
	// Calculate attenuation for each light
	for(size_t lightIndex = 1; lightIndex < m_lights.size(); ++lightIndex)
	{
		Vector3 lightDir = m_lights[lightIndex]->GetWorldPosition() - position;
		Vector3& atten = m_lights[lightIndex]->m_lightDesc.attenuation;
		float distance = lightDir.GetLength();
		float distaceSq = distance * distance;
		float lightPower = m_lights[lightIndex]->m_lightDesc.color.a * 1.f / ( atten.x + distance * atten.y + distaceSq * atten.z );
		lightPowers.push_back(lightPower);
	}

	// Sort the lights
	for(size_t lightI = 0; lightI < m_lights.size(); ++ lightI)
	{
		for(size_t lightJ = lightI + 1; lightJ < m_lights.size(); ++lightJ)
		{
			if(lightPowers[lightI] > lightPowers[lightJ])
			{
				std::swap(lightPowers[lightI], lightPowers[lightJ]);
				std::swap(m_lights[lightI], m_lights[lightJ]);
			}
		}
	}

	// Make a vector of contributing lights and return it
	std::vector<Light*> lights;
	for(int index = 0; index < MAX_LIGHTS; ++index)
	{
		lights.push_back(m_lights[index]);
	}
	return lights;
}

//-----------------------------------------------------------------------------------------------
// Adds a renderable to the scene
//
void RenderScene::AddRenderable(Renderable* r)
{
	m_renderables.push_back(r);
}

//-----------------------------------------------------------------------------------------------
// Adds a light to the scene
//
void RenderScene::AddLight(Light* light)
{
	m_lights.push_back(light);
}

//-----------------------------------------------------------------------------------------------
// Adds a camera to the scene
//
void RenderScene::AddCamera(Camera* cam)
{
	m_cameras.push_back(cam);
}

//-----------------------------------------------------------------------------------------------
// Adds a prerender callback to the list
//
void RenderScene::AddPreRender(PrerenderCB cb)
{
	m_prerenders.push_back(cb);
}

//-----------------------------------------------------------------------------------------------
// Adds camera independent prerenders to the list
//
void RenderScene::AddPreRenderIndie(PrerenderIndieCB cb)
{
	m_indiePrerenders.push_back(cb);
}

//-----------------------------------------------------------------------------------------------
// Removes the renderable from the scene
//
void RenderScene::RemoveRenderable(Renderable* r)
{
	for(size_t index = 0; index < m_renderables.size(); ++index)
	{
		if(m_renderables[index] == r)
		{
			m_renderables[index] = m_renderables[m_renderables.size() - 1];
			m_renderables.pop_back();
			index--;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Removes the light from the list
//
void RenderScene::RemoveLight(Light* light)
{
	for(size_t index = 0; index < m_lights.size(); ++index)
	{
		if(m_lights[index] == light)
		{
			m_lights[index] = m_lights[m_lights.size() - 1];
			m_lights.pop_back();
			index--;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Removes a camera from the scene
//
void RenderScene::RemoveCamera(Camera* cam)
{
	for(size_t index = 0; index < m_cameras.size(); ++index)
	{
		if(m_cameras[index] == cam)
		{
			m_cameras[index] = m_cameras[m_cameras.size() - 1];
			m_cameras.pop_back();
			index--;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Removes a prerender cb from the scene
//
void RenderScene::RemovePreRender(PrerenderCB cb)
{
// 	std::vector<PrerenderCB>::iterator iter = std::find(m_prerenders.begin(), m_prerenders.end(), cb);
// 	
// 	if(iter != m_prerenders.end())
// 	{
// 		*iter = m_prerenders[m_prerenders.size() - 1];
// 		m_prerenders.pop_back();
// 	}
}

//-----------------------------------------------------------------------------------------------
// Removes an independent pre render from the scene
//
void RenderScene::RemovePreRenderIndie(PrerenderIndieCB cb)
{
// 	std::vector<PrerenderIndieCB>::iterator iter = std::find(m_indiePrerenders.begin(), m_indiePrerenders.end(), cb);
// 
// 	if(iter != m_indiePrerenders.end())
// 	{
// 		*iter = m_indiePrerenders[m_indiePrerenders.size() - 1];
// 		m_indiePrerenders.pop_back();
// 	}
}
