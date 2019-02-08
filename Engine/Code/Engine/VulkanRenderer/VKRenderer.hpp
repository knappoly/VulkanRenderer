#pragma once
#define VK_NO_PROTOTYPES
#include "Engine/VulkanRenderer/External/Vulkan/vulkan.h"
#include "Engine/Enumerations/ReservedDescriptorSetSlot.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Enumerations/TextureFormat.hpp"
#include "Engine/Enumerations/ShaderStageSlot.hpp"
#include <vector>
#include <map>

//-----------------------------------------------------------------------------------------------
// Constants
constexpr int QUEUE_FAMILY_INDICES_MAX = 16;
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

//-----------------------------------------------------------------------------------------------
// Forward Declarations
struct Vertex_3DPCU;
class Matrix44;
enum VKRenderType;
enum eTextureFormat;
class VKVertexBuffer;
class VKIndexBuffer;
class VKTexture;
class VKShaderProgram;
class Image;
class VKTexSampler;
class VKShader;
class VKCamera;
class VKMaterial;
class VKMesh;
class VKPipeline;
class VKUniformBuffer;
class VKCamera;
struct VertexLayout;
struct RenderState;
enum DrawPrimitiveType;
enum eTextureFormat;
enum RenderType;
enum BlendFactor;
enum BlendOp;
enum DepthTestOp;
enum FillMode;
enum CullMode;
enum WindOrder;

//-----------------------------------------------------------------------------------------------
struct QueueFamilyIndices // Stores the index of the actual queue that needs to be used when using operations. 
{
	//-----------------------------------------------------------------------------------------------
	// Methods
	bool IsComplete() { return graphicsFamily >= 0 && presentFamily >= 0; }
	bool IsGraphicsQueueValid() { return graphicsFamily >= 0; }
	bool IsPresentationSupported() { return presentFamily >= 0; }
	
	//-----------------------------------------------------------------------------------------------
	// Members
	int				graphicsFamily = -1; // More members will be added for other queue family support
	int				presentFamily = -1; 
};

//-----------------------------------------------------------------------------------------------
struct SwapChainDetails // Stores the details of the swapchain supported by the physical device
{
	// Members
	VkSurfaceCapabilitiesKHR		capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR>	presentModes;
};

//-----------------------------------------------------------------------------------------------
struct MatrixBuffer
{
	Matrix44 MODEL;
	Matrix44 VIEW;
	Matrix44 PROJECTION;
};

//-----------------------------------------------------------------------------------------------
struct ModelBuffer
{
	Matrix44 MODEL;
};

//-----------------------------------------------------------------------------------------------
class VKRenderer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKRenderer( const char* appName );
	~VKRenderer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			VkDevice			GetLogicalDevice() const { return m_logicalDevice; }
			VkPhysicalDevice		GetPhysicalDevice() const { return m_physicalDevice; }
			VKTexture*			GetDefaultColorTarget() const { return m_defaultColorTarget; }
			VKTexture*			GetDefaultDepthTarget() const { return m_defaultDepthTarget; }
	
	//-----------------------------------------------------------------------------------------------
	// Vulkan Initialization Operations
private:
			bool				CheckValidationLayerSupport();
			bool				CheckExtensionsSupport();
			void				SetupDebugCallback();
			void				InitializeVulkanInstance( const char* appName );
			bool				CheckDeviceExtensionsSupport( VkPhysicalDevice device );
			QueueFamilyIndices		GetQueueFamilyIndices( VkPhysicalDevice device );
			SwapChainDetails		GetSwapChainDetails( VkPhysicalDevice device );
			bool				IsDeviceSuitable( VkPhysicalDevice device );
			void				CreateSurface();
			void				PickPhysicalDevice();
			void				CreateLogicalDevice();
			VkSurfaceFormatKHR		PickSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& formats );
			VkPresentModeKHR		PickPresentMode( const std::vector<VkPresentModeKHR>& presentModes );
			VkExtent2D			PickSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );
			void				CreateSwapChain();
			void				CreateImageViews();
			void				CreateCommandPool();
			void				CreateVertexBuffer();
			void				CreateIndexBuffer();
			void				CreateSyncStuff();
			void				CleanupSwapchain();
			void				RecreateSwapchain();

	//-----------------------------------------------------------------------------------------------
	// Methods
public:
			void				PostStartup();
			void				BeginFrame();
			void				EndFrame();

	//-----------------------------------------------------------------------------------------------
	// Draw commands
			void				DrawMeshImmediate(const Vertex_3DPCU* vertices, int numVerts, DrawPrimitiveType mode, const Matrix44& modelMatrix);
			void				DrawMesh( const VKMesh& mesh, const Matrix44& modelMatrix = Matrix44::IDENTITY );

	//-----------------------------------------------------------------------------------------------
	// Mesh functions
			VKMesh*				CreateOrGetMesh( const std::string& path );
			void				InitializeDefaultMeshes();
	//-----------------------------------------------------------------------------------------------
	// Command Buffer ops
			VkCommandBuffer			BeginTemporaryCommandBuffer(); // Begins a command buffer for temp usage and returns the handle
			void				EndTemporaryCommandBuffer( VkCommandBuffer tempBuffer ); 

	//-----------------------------------------------------------------------------------------------
	// Texture Ops
			void				CreateAndGetImage( VkImage* out_image, VkDeviceMemory* out_devMem, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImageTiling tiling, VkMemoryPropertyFlags props );
			VkImageView			CreateAndGetImageView( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags );
			void				TransitionImageLayout( VkImage image, VkImageAspectFlags aspectFlags, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkAccessFlags srcMask, VkAccessFlags dstMask );
			void				CopyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height );
			void				CopyImages( VkImage dst, VkImageLayout dstLayout, VkImage src, VkImageLayout srcLayout, VkImageCopy copyInfo, VkSemaphore* waitSemaphore = nullptr, uint32_t waitCount = 0, VkSemaphore* signalSemaphores = nullptr, uint32_t signalCount = 0 );

	//-----------------------------------------------------------------------------------------------
	// Texture Helpers
			VKTexture*			GetDefaultTexture() const { return m_defaultTexture; }
			void				SetTexture( VKTexture* texture );
			void				SetTexture( unsigned int index, VKTexture* texture, VKTexSampler* sampler = nullptr );
			void				BindTexture2D( const VKTexture* texture );
			void				BindTexture2D( unsigned int index, const VKTexture* texture );
			VKTexture*			CreateOrGetTexture(const std::string& path, bool genMipmaps = true);
			VKTexture*			CreateOrGetTexture(const Image& image, bool genMipmaps = true);
			bool				IsTextureLoaded(const std::string& path) const;
			void				SetDefaultTexture();
			void				CopyTexture2D( VKTexture* dst, VKTexture* src, VkSemaphore* waitSemaphore = nullptr, uint32_t waitCount = 0, VkSemaphore* signalSemaphores = nullptr, uint32_t signalCount = 0 );
			VKTexture*			CreateRenderTarget(unsigned int width, unsigned int height, eTextureFormat fmt = TEXTURE_FORMAT_RGBA8);
			VKTexture*			CreateDepthStencilTarget( unsigned int width, unsigned int height );
			VKTexture*			CreateColorTarget( unsigned int width, unsigned int height );

	//-----------------------------------------------------------------------------------------------
	// Shader functions
			void				SetShader( VKShader* shader = nullptr );
			VKShaderProgram*		CreateOrGetShaderProgram(const std::string& path, const char* defines = nullptr);
			void				UseShaderProgram(const VKShaderProgram* shaderProgram);
			void				SetDefaultShader();
			void				BindMeshToProgram( const VKMesh* mesh );
			void				BindRenderState( RenderState state );
			void				AlphaBlendFunction(BlendFactor sfactor, BlendFactor dfactor );
			void				ColorBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
			void				SetDepthTestMode(DepthTestOp mode, bool flag);
			void				DisableDepth();
			void				SetFillMode(FillMode fillMode);
			void				SetCullMode(CullMode cullMode);

	//-----------------------------------------------------------------------------------------------
	// Material Functions
			void				BindMaterial( const VKMaterial* material = nullptr );
			void				SetMaterial( const VKMaterial* material = nullptr );
			VKMaterial*			CreateOrGetMaterial( const std::string& path );
			void				SetDefaultMaterial();
			void				ResetDefaultMaterial();

	//-----------------------------------------------------------------------------------------------
	// Setting uniforms on shaders
			void				BindUBO( int bindPoint, const VKUniformBuffer* ubo );
			void				SetUniform( const char* name, float value );
			void				SetUniform( const char* name, int value );
			void				SetUniform( const char* name, const Rgba& color );
			void				SetUniform( const char* name, const Matrix44& matrix, bool transpose = false );
			void				SetUniform( const char* name, const Vector3& value );

	//-----------------------------------------------------------------------------------------------
	// Camera Functions
			void				SetCamera(VKCamera* cam);

	//-----------------------------------------------------------------------------------------------
	// Buffer operations
			uint32_t			FindMemoryType( uint32_t requiredTypes, uint32_t requiredProps ) const;
			void				CreateAndGetBuffer( VkBuffer* out_buffer, VkDeviceMemory* out_deviceMem, 
									   VkDeviceSize size, VkBufferUsageFlags usage, 
									   VkMemoryPropertyFlags props );
			void				CopyBuffers( VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize byteCount );
	
	//-----------------------------------------------------------------------------------------------
	// Static methods
	static		VKRenderer*			CreateInstance( const char* appName );
	static		void				DestroyInstance();
	static		VKRenderer*			GetInstance();

	// Debug callback
	static	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);

	//-----------------------------------------------------------------------------------------------
	// Vulkan Members
private:
			uint32_t					m_currentFrame = 0;
			VkDebugReportCallbackEXT			m_debugCallback;
			VkInstance					m_vkInstance;
			VkPhysicalDevice				m_physicalDevice = VK_NULL_HANDLE;
			VkDevice					m_logicalDevice = VK_NULL_HANDLE;
			VkQueue						m_graphicsQueue;
			VkSurfaceKHR					m_surface;
			VkQueue						m_presentQueue;
			VkSwapchainKHR					m_swapChain;
			VkExtent2D					m_swapChainExtent;
			VkFormat					m_swapChainImageFormat;
			std::vector<VkImage>				m_swapChainImages;
			std::vector<VkImageView>			m_swapChainImageViews;
			VkRenderPass					m_renderPass;
			VkPipelineLayout				m_pipelineLayout;
			VkPipeline					m_graphicsPipeline;
			std::vector<VkFramebuffer>			m_framebuffers;
			VkCommandPool					m_commandPool;
			std::vector<VkCommandBuffer>			m_commandBuffers;
			std::vector<VkSemaphore>			m_imageAvailableSemaphore;
			std::vector<VkSemaphore>			m_renderFinishedSemaphore;
			std::vector<VkSemaphore>			m_colorTargetAvailableSemaphore;
			std::vector<VkFence>				m_fences;
	
	//-----------------------------------------------------------------------------------------------
	// Data Members
			std::map<std::string,VKTexture*>		m_loadedTextures; 
			std::map<std::string,VKShaderProgram*>		m_loadedShaderPrograms;
			std::map<std::string,VKMesh*>			m_loadedMeshes;
			std::map<std::string,VKMaterial*>		m_loadedMaterials;
			VKVertexBuffer*					m_immediateVBO;
			VKIndexBuffer*					m_immediateIBO;
			VKTexture*					m_immediateTexture = nullptr;
			VKTexture*					m_defaultTexture = nullptr;
			VkDescriptorSetLayout				m_descriptorSetLayout;
			VkDescriptorPool				m_descriptorPool;
			VKCamera*					m_defaultCamera = nullptr;
			VKCamera*					m_defaultPerspectiveCamera = nullptr;
			VKCamera*					m_currentCamera = nullptr;
			VKTexture*					m_defaultColorTarget = nullptr;
			VKTexture*					m_defaultDepthTarget = nullptr;
			VKShader*					m_defaultShader = nullptr;
			VKMaterial*					m_defaultMaterial = nullptr;
			VKMaterial*					m_defaultMaterialShared = nullptr;
			VKMaterial*					m_activeMaterial = nullptr;
			VKPipeline*					m_defaultPipeline = nullptr;
			VKUniformBuffer*				m_testBuffer = nullptr;
			VKUniformBuffer*				m_modelBuffer = nullptr;
			uint32_t					m_swapImageIndex = 0;
			VkBuffer					m_ubo;
			VkDeviceMemory					m_uboMemory;
			VkDescriptorSet					m_descriptorSet;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
void				VkRenderStartup();
void				VkShutdown();
VkFormat			GetVKDataType( VKRenderType type );
VkFormat			GetVkFormat( eTextureFormat format );
VkShaderStageFlagBits		GetVKShaderStageFlag( ShaderStageSlot stage );
VkPrimitiveTopology		GetVKDrawType( DrawPrimitiveType type );
VkPolygonMode			GetVKPolygonMode( FillMode mode );
VkCullModeFlags			GetVKCullMode( CullMode mode );
VkFrontFace			GetVKWindOrder( WindOrder order );
VkCompareOp			GetVKDepthOp( DepthTestOp compare );
VkBlendOp			GetVKBlendOp( BlendOp op );
VkBlendFactor			GetVKBlendFactor( BlendFactor factor );
