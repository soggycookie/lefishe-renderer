#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "global_header.h"
#include "component.h"

namespace Lefishe{

	class Object;


	class ObjectLoader {
	public:
		virtual std::shared_ptr<Object> loadObject(const std::string& path) = 0;

		//static void processNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Object>& model_ptr);
	};

	class AssimpObjectLoader : public ObjectLoader {
	public:
		std::shared_ptr<Object> loadObject(const std::string& path) override;
	
	private:
		void processNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Object> model_ptr);
	
	private:
		Assimp::Importer m_importer;
	};

}