#pragma once



#include "global_header.h"

namespace Lefishe{
	class Texture;

	class TextureLoader{
	public:
		static std::shared_ptr<Texture> load(const STRING& path);
	};

}