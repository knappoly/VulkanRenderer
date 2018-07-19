#pragma once
#include "Engine/Enumerations/TextureFormat.hpp"
#include <string>
#include "Engine/Math/IntVector2.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Image;
class VKTexSampler;
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKTexture
{
	friend class VKRenderer;

private:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKTexture( VKRenderer& renderer );
	VKTexture( VKRenderer& renderer, const std::string& imageFilePath, VKTexSampler* sampler = nullptr ); // Use VkRenderer->CreateOrGetTexture() instead!
	VKTexture( VKRenderer& renderer, const Image& image, VKTexSampler* sampler = nullptr );
	~VKTexture();

public:
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void*			GetHandle() const { return m_texHandle; }
	void*			GetImageViewHandle() const { return m_viewHandle; }
	void*			GetSamplerHandle() const;
	int				GetLayout() const;
	IntVector2		GetDimensions() const { return m_dimensions; }
	eTextureFormat	GetFormat() const { return m_format; }
	int				GetVulkanFormat() const;
	void			SetSampler( VKTexSampler* sampler );
	VKTexSampler*	GetSampler() const { return m_sampler; }

private:
	//-----------------------------------------------------------------------------------------------
	// Methods
	void			CreateRenderTarget( int width, int height, eTextureFormat format );
	void			CreateColorTarget( int width, int height, eTextureFormat format );
	void			CreateDepthTarget( int width, int height, eTextureFormat format );
	void			LoadFromImage( const Image& image );
	void			PopulateFromData( unsigned char* imageData, const IntVector2& texelSize, int numComponents );

	//-----------------------------------------------------------------------------------------------
	// Members
private:
			VKRenderer&									m_renderer;
			void*										m_texHandle;
			void*										m_memHandle;
			void*										m_viewHandle;
			int											m_imageLayout;
			IntVector2									m_dimensions;
			eTextureFormat								m_format = TEXTURE_FORMAT_RGBA8;
			VKTexSampler*								m_sampler = nullptr;
};

