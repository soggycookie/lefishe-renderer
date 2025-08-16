#pragma once



#include "global_header.h"

namespace Lefishe{
	class Texture;

	class TextureLoader{
	public:
		std::shared_ptr<Texture> load(const char* path);
	};

}