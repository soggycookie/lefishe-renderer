#pragma once

#include <assimp/scene.h>


#include "global_header.h"
#include "component.h"
#include "material_manager.h"
#include "texture_manager.h"

namespace Lefishe{

	class Object;


	class ObjectLoader {
	public:
		virtual ~ObjectLoader() = default;
		ObjectLoader(const std::shared_ptr<MaterialManager>& mat_manager, const std::shared_ptr<TextureManager>& tex_manager);

		virtual std::shared_ptr<Object> loadObject(const std::string& path) = 0;

	protected:
		std::weak_ptr<MaterialManager> m_material_manager;
		std::weak_ptr<TextureManager> m_texture_manager;
	};

	class AssimpObjectLoader : public ObjectLoader {
	public:
		AssimpObjectLoader(const std::shared_ptr<MaterialManager>& mat_manager, const std::shared_ptr<TextureManager>& tex_manager);

		std::shared_ptr<Object> loadObject(const std::string& path) override;
	
	private:
		void processNode(aiNode* node, const aiScene* scene, const STRING& dir, const std::shared_ptr<Object> parent_ptr);
		void processMaterial(aiMesh* mesh, const aiScene* scene, const STRING& dir, const std::shared_ptr<Material> obj_material);
		void processTexture(aiMaterial* material, const aiScene* scene, aiTextureType type, const STRING& dir, const std::shared_ptr<Material> obj_material);
		STRING textureEnumToString(aiTextureType type);
		void assignTextureToMaterial(aiTextureType type, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Material>& obj_material);

	private:

	};

}