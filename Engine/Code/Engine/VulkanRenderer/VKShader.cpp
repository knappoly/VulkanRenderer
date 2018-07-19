#include "Engine/VulkanRenderer/VKShader.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/VulkanRenderer/VKShaderProgram.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
//-----------------------------------------------------------------------------------------------

typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Static globals
std::map<std::string, VKShader*> VKShader::m_loadedShaders;

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKShader::VKShader(VKShaderProgram* program, VKRenderer* renderer)
{
	m_program = program; 
	m_renderer = renderer;

	CreateDescriptorSets();
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKShader::VKShader(const tinyxml2::XMLElement& element, VKRenderer* renderer)
{
	m_renderer = renderer;

	const XMLElement* defElement = element.FirstChildElement("cull");
	// Cull Mode
	if(defElement)
	{
		std::string cullMode = "INVALID";
		cullMode = ParseXmlAttribute(*defElement, "mode", cullMode);
		GUARANTEE_OR_DIE(cullMode != "INVALID", "Cull mode invalid specification");
		m_renderState.m_cullMode = ParseCullMode.at(cullMode);
	}

	// Fill Mode
	defElement = element.FirstChildElement("fill");
	if(defElement)
	{
		std::string fillMode = "INVALID";
		fillMode = ParseXmlAttribute(*defElement, "mode", fillMode);
		GUARANTEE_OR_DIE(fillMode != "INVALID", "Fill mode invalid specification");
		m_renderState.m_fillMode = ParseFillMode.at(fillMode);
	}

	// Wind order
	defElement = element.FirstChildElement("windorder");
	if(defElement)
	{
		std::string windOrder = "INVALID";
		windOrder = ParseXmlAttribute(*defElement, "order", windOrder);
		GUARANTEE_OR_DIE(windOrder != "INVALID", "Wind order invalid specification");
		m_renderState.m_frontFace = ParseWindOrder.at(windOrder);
	}

	// Shader program 
	defElement = element.FirstChildElement("program");
	if(defElement)
	{
		const char* defines = nullptr;
		defines = ParseXmlAttribute(*defElement, "define", defines);

		// Program element has the src so the program cannot have defines and stage definitions
		if(defElement->FindAttribute("src"))
		{
			std::string programFilesPath = "INVALID";
			programFilesPath = ParseXmlAttribute(*defElement, "src", programFilesPath);
			GUARANTEE_OR_DIE(programFilesPath != "INVALID", "Shader programs path invalid specification");
			defines = ParseXmlAttribute(*defElement, "define"); // Checking if defines specified to throw warning
			GUARANTEE_RECOVERABLE(defines == nullptr, "Skipping shader defines as program id and defines are mutually exclusive");

			// Loads the program files by adding extensions for vs and fs
			m_program = m_renderer->CreateOrGetShaderProgram(programFilesPath);
		}
		else
		{
			// Separate vertex and fragment stages specified. Thus program instance remains with this shader instance
			// Vertex stage
			const XMLElement* stageElement = defElement->FirstChildElement("vertex");
			GUARANTEE_OR_DIE(stageElement != nullptr, "Vertex shader path not specified for Shader");
			std::string vsPath = "INVALID";
			vsPath = ParseXmlAttribute(*stageElement, "file", vsPath);
			GUARANTEE_OR_DIE(vsPath != "INVALID", "Vertex shader path not specified for Shader");

			// Fragment stage
			stageElement = defElement->FirstChildElement("fragment");
			GUARANTEE_OR_DIE(stageElement != nullptr, "fragment shader path not specified for Shader");
			std::string fsPath = "INVALID";
			fsPath = ParseXmlAttribute(*stageElement, "file", fsPath);
			GUARANTEE_OR_DIE(fsPath != "INVALID", "Fragment shader path not specified for Shader");

			VKShaderProgram* program = new VKShaderProgram(m_renderer);

			if(program->LoadFromFiles(vsPath.c_str(), fsPath.c_str(), defines))
			{
				m_program = program;
			}
			else
			{
				GUARANTEE_OR_DIE(false, "Shader compilation error. Cannot create Shader object");
			}
		}

	}

	// Depth state
	defElement = element.FirstChildElement("depth");
	if(defElement)
	{
		m_renderState.m_depthWrite = ParseXmlAttribute(*defElement, "write", m_renderState.m_depthWrite);
		std::string depthTest = "INVALID";
		depthTest = ParseXmlAttribute(*defElement, "test", depthTest);
		GUARANTEE_OR_DIE(depthTest != "INVALID", "Depth test invalid specification");
		m_renderState.m_depthCompare = ParseDepthTest.at(depthTest);
	}


	defElement = element.FirstChildElement("blend");
	if(defElement)
	{
		// Alpha blending state
		const XMLElement* blendElement = defElement->FirstChildElement("alpha");
		if(blendElement)
		{
			std::string op = "INVALID";
			op = ParseXmlAttribute(*blendElement, "op", op);
			GUARANTEE_OR_DIE(op != "INVALID", "Alpha blend op invalid specification");
			m_renderState.m_alphaBlendOp = ParseBlendOp.at(op);

			std::string src = "INVALID";
			src = ParseXmlAttribute(*blendElement, "src", src);
			GUARANTEE_OR_DIE(src != "INVALID", "Alpha blend src invalid specification");
			m_renderState.m_alphaSrcFactor = ParseBlendFactor.at(src);

			std::string dst = "INVALID";
			dst = ParseXmlAttribute(*blendElement, "dst", dst);
			GUARANTEE_OR_DIE(src != "INVALID", "Alpha blend dst invalid specification");
			m_renderState.m_alphaDstFactor = ParseBlendFactor.at(dst);
		}

		// Color blending state
		blendElement = defElement->FirstChildElement("color");
		if(blendElement)
		{
			std::string op = "INVALID";
			op = ParseXmlAttribute(*blendElement, "op", op);
			GUARANTEE_OR_DIE(op != "INVALID", "Color blend op invalid specification");
			m_renderState.m_colorBlendOp = ParseBlendOp.at(op);

			std::string src = "INVALID";
			src = ParseXmlAttribute(*blendElement, "src", src);
			GUARANTEE_OR_DIE(src != "INVALID", "Color blend src invalid specification");
			m_renderState.m_colorSrcFactor = ParseBlendFactor.at(src);

			std::string dst = "INVALID";
			dst = ParseXmlAttribute(*blendElement, "dst", dst);
			GUARANTEE_OR_DIE(src != "INVALID", "Color blend dst invalid specification");
			m_renderState.m_colorDstFactor = ParseBlendFactor.at(dst);
		}
	}

	defElement = element.FirstChildElement("queue");
	if(defElement)
	{
		std::string queueStr = "INVALID";
		queueStr = ParseXmlAttribute(*defElement, "name", queueStr);
		GUARANTEE_OR_DIE(queueStr != "INVALID", "Render Queue name invalid specification");
		m_renderQueue = ParseRenderQueue.at(queueStr);
	}

	defElement = element.FirstChildElement("order");
	if(defElement)
	{
		m_sortOrder = 0;
		m_sortOrder = ParseXmlAttribute(*defElement, "value", m_sortOrder);
	}

	CreateDescriptorSets();
}

//-----------------------------------------------------------------------------------------------
// Creates the descriptor sets that use the pools
//
void VKShader::CreateDescriptorSets()
{
	const std::vector<void*> pools = m_program->GetDescPools();
	const std::vector<void*> layouts = m_program->GetDescSetLayouts();

	m_descriptorSets.resize(layouts.size());
	for(int setIndex = 0; setIndex < layouts.size(); ++setIndex)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorSetCount = 1;
		allocInfo.descriptorPool = (VkDescriptorPool) pools[setIndex];
		allocInfo.pSetLayouts = (VkDescriptorSetLayout*) &layouts[setIndex];

		if(vkAllocateDescriptorSets(m_renderer->GetLogicalDevice(), &allocInfo, (VkDescriptorSet*) &m_descriptorSets[setIndex]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Couldn't allocate descriptor set");
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Sets alpha blending mode 
//
void VKShader::SetAlphaBlending(BlendOp op, BlendFactor src, BlendFactor dst)
{
	m_renderState.m_alphaBlendOp = op;
	m_renderState.m_alphaSrcFactor = src;
	m_renderState.m_alphaDstFactor = dst;
}

//-----------------------------------------------------------------------------------------------
// Disables alpha blending 
//
void VKShader::DisableAlphaBlending()
{
	SetAlphaBlending(BLEND_OP_ADD, BLEND_ONE, BLEND_ONE);
}

//-----------------------------------------------------------------------------------------------
// Sets color blending mode
//
void VKShader::SetColorBlending(BlendOp op, BlendFactor src, BlendFactor dst)
{
	m_renderState.m_colorBlendOp = op;
	m_renderState.m_colorSrcFactor = src;
	m_renderState.m_colorDstFactor = dst;
}

//-----------------------------------------------------------------------------------------------
// Disables color blending mode
//
void VKShader::DisableColorBlending()
{
	SetColorBlending(BLEND_OP_ADD, BLEND_ONE, BLEND_ZERO);
}

//-----------------------------------------------------------------------------------------------
// Sets depth test mode
//
void VKShader::SetDepthTest(DepthTestOp compare, bool write)
{
	m_renderState.m_depthCompare = compare;
	m_renderState.m_depthWrite = write;
}

//-----------------------------------------------------------------------------------------------
// Returns a new copy of an existing shader or loads from file
//
VKShader* VKShader::AcquireResource(const std::string& path)
{
	std::map<std::string, VKShader*>::iterator found = m_loadedShaders.find(path);

	if(found != m_loadedShaders.end())
	{
		// Creates an instance and returns it
		VKShader* instance = new VKShader(*found->second);
		return instance;
	}
	else
	{
		std::string xmlPath = path + ".xml";
		XMLDocument file;
		file.LoadFile(xmlPath.c_str());

		XMLElement* root = file.FirstChildElement();
		VKShader* instance = new VKShader(*root, VKRenderer::GetInstance());
		m_loadedShaders[path] = instance;

		// Return a copy of the new shader just created
		return new VKShader(*instance);
	}
}

//-----------------------------------------------------------------------------------------------
// Returns a pointer to an existing shader or loads from file (shared resource)
//
VKShader* VKShader::CreateOrGetResource(const std::string& path)
{
	std::map<std::string, VKShader*>::iterator found = m_loadedShaders.find(path);

	if(found != m_loadedShaders.end())
	{
		return found->second;
	}
	else
	{
		std::string xmlPath = path + ".xml";
		XMLDocument file;
		file.LoadFile(xmlPath.c_str());

		XMLElement* root = file.FirstChildElement();
		VKShader* newShader = new VKShader(*root, VKRenderer::GetInstance());
		m_loadedShaders[path] = newShader;

		return newShader;
	}
}
