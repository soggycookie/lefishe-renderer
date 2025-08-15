#include "texture_loader.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Lefishe;

 std::shared_ptr<Texture> TextureLoader::load(const STRING& path){
	INT tex_width  = 0;
	INT tex_height = 0;
	INT channels = 0;

	std::shared_ptr<Texture> texture = nullptr;

	stbi_set_flip_vertically_on_load(true);  
	LOG_INFO("texture dir : {0} ", path);
	unsigned char *data = stbi_load(path.c_str(), &tex_width, &tex_height, &channels, 0);
	if (data)
	{
		TextureInternalFormat internal_format;
		TexturePixelFormat format;
		TexturePixelFormat type;
		if(channels == 3){
			internal_format = TextureInternalFormat::RGB8;
			format = TexturePixelFormat::RGB;
		}else if(channels == 4){
			internal_format = TextureInternalFormat::RGBA8;
			format = TexturePixelFormat::RGBA;
		}

		//UINT mip = std::log2(std::max(width, height) / 32);

		TextureInfo info = 	
		{
			.type = TextureType::TEX_2D,
			.internal_format = internal_format,
			.width = static_cast<UINT>(tex_width),
			.height = static_cast<UINT>(tex_height),
			.total_mip_levels = 1
		};

		texture = std::make_shared<Texture>(info);

		texture->subDataTex2D(0, 0, 0, tex_width, tex_height, format, TexturePixelType::UBYTE, data);

		LOG_INFO("width: {0} | height: {1} | channels: {2}", tex_width, tex_height, channels);
	}
	else
	{
		LOG_WARN("Texture at {0} can be loaded!", path);
	}
	stbi_image_free(data);

	return texture;
 }
