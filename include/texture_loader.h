#pragma once



#include "global_header.h"

namespace Lefishe{
	class Texture2D;

	class TextureLoader{
	public:
		std::shared_ptr<Texture2D> load(const char* path);
	};

}