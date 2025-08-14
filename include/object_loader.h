#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "global_header.h"
#include "component.h"
#include "material_manager.h"

namespace Lefishe{

	class Object;


	class ObjectLoader {
	public:
		virtual ~ObjectLoader() = default;
		ObjectLoader(const std::shared_ptr<MaterialManager>& manager);

		virtual std::shared_ptr<Object> loadObject(const std::string& path) = 0;

	protected:
		std::weak_ptr<MaterialManager> m_manager;
	};

	class AssimpObjectLoader : public ObjectLoader {
	public:
		AssimpObjectLoader(const std::shared_ptr<MaterialManager>& manager);

		std::shared_ptr<Object> loadObject(const std::string& path) override;
	
	private:
		void processNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Object> model_ptr);
	
	private:
		Assimp::Importer m_importer;
	};

}