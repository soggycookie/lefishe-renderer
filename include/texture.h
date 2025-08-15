#pragma once
#include "global_header.h"

namespace Lefishe{

	enum class TextureType{
		TEX_2D = GL_TEXTURE_2D
	};

	enum class TextureInternalFormat{
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8,
		RGB_16F = GL_RGB16F,
		RGB_32F = GL_RGB32F,
		RGBA_16F = GL_RGBA16F,
		RGBA_32F = GL_RGBA32F,
	};

	enum class TexturePixelFormat{
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH = GL_DEPTH_COMPONENT
	};

	enum class TexturePixelType{
		FLOAT = GL_FLOAT,
		UINT = GL_UNSIGNED_INT,
		UBYTE = GL_UNSIGNED_BYTE
	};

	struct TextureInfo{
		TextureType type = TextureType::TEX_2D;
		TextureInternalFormat internal_format = TextureInternalFormat::RGBA8;
		UINT width  = 0;
		UINT height = 0;
		UINT total_mip_levels = 1;
	};

	enum class SamplerMode{
		WRAP_S = GL_TEXTURE_WRAP_S,
		WRAP_T = GL_TEXTURE_WRAP_T,
		MIN_FILTER = GL_TEXTURE_MIN_FILTER,
		MAX_FILTER = GL_TEXTURE_MAG_FILTER
	};

	enum class SamplerValue{
		NEAREST = GL_NEAREST,
		LINEAR  = GL_LINEAR
	};


	class Texture{
	public:
		Texture(TextureInfo info);
		
		void subDataTex2D(UINT mip_level, INT x_offset, INT y_offset, UINT width, UINT height, TexturePixelFormat format, TexturePixelType type, const void* data );

		//void setSamplerData(SamplerMode mode, SamplerValue value);
		void bind(UINT index);

		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;
	private:
		void create();
		void samplerMode();

	private:

		UINT m_id     = 0;
		TextureInfo m_info;
	};

	//class TextureManager;
}