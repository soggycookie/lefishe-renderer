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

		DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
		DEPTH24 = GL_DEPTH_COMPONENT24
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

	struct Texture2DInfo{
		TextureInternalFormat internal_format = TextureInternalFormat::RGBA8;
		UINT width  = 0;
		UINT height = 0;
		UINT total_mip_levels = 1;

		bool operator==(const Texture2DInfo&) const = default;
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
		void bind(UINT index) const;

		static inline void unbind(UINT binding_index){
			glBindTextureUnit(binding_index, 0);
		}

		UINT id() const;
	private:
		virtual void create() = 0;

	protected:
		UINT m_tex_id = 0;
	};

	
	//class RenderTexture : public Texture{
	//public:
	//	RenderTexture() = default;
	//	RenderTexture(Texture2DInfo info);

	//	void create(Texture2DInfo info);

	//private:
	//	void create() override;
	//	
	//private:
	//	Texture2DInfo m_info;
	//};

	
	class Texture2D : public Texture{
	public:
		Texture2D(Texture2DInfo info);
		
		void subDataTex2D(UINT mip_level, INT x_offset, INT y_offset, UINT width, UINT height, TexturePixelFormat format, TexturePixelType type, const void* data );

		Texture2D& operator=(const Texture2D& other) = delete;
		Texture2D& operator=(Texture2D&& other) = delete;
	private:
		void create() override;
		void samplerMode();

	private:
		Texture2DInfo m_info;
	};

	//class TextureManager;
}