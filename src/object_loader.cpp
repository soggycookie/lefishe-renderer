#include "object_loader.h"
#include "object.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace Lefishe;
	
	ObjectLoader::ObjectLoader(const std::shared_ptr<MaterialManager>& mat_manager, const std::shared_ptr<TextureManager>& tex_manager)
		: m_material_manager(mat_manager), m_texture_manager(tex_manager)
	{
	}

	AssimpObjectLoader::AssimpObjectLoader(const std::shared_ptr<MaterialManager>& mat_manager, const std::shared_ptr<TextureManager>& tex_manager)
		: ObjectLoader(mat_manager, tex_manager)
	{
	}

	std::shared_ptr<Object> AssimpObjectLoader::loadObject(const std::string& path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_CalcTangentSpace);
		
		if (scene == nullptr) {
			LOG_WARN("[ASSIMP] Scene is null! Check the asset path!");
			return nullptr;
		}

		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			LOG_WARN("[ASSIMP] {0}", importer.GetErrorString());
			return nullptr;
		}

		if (!scene->mRootNode) {
			LOG_WARN("[ASSIMP] Scene is empty!");
			return nullptr;
		}

		std::shared_ptr<Object> parent_ptr = ObjectFactory::create();

		std::string directory = path.substr(0, path.find_last_of('/'));
		LOG_TRACE("\n");
		LOG_TRACE("{0}", path);
		
		processNode(scene->mRootNode, scene, directory, parent_ptr);

		return parent_ptr;
	}

	void AssimpObjectLoader::processNode(aiNode* node, const aiScene* scene, const STRING& dir, const std::shared_ptr<Object> parent_ptr) {

		MeshData data;
		bool has_data = true;;

		aiMatrix4x4 node_transform = node->mTransformation;
		MAT4 rel_mtx(1.0f);
		std::memcpy(&rel_mtx, &node_transform, sizeof(MAT4));
		rel_mtx = glm::transpose(rel_mtx);

		//static int c = 1;
		//LOG_ERROR("{0}", c++);


		parent_ptr->transform()->localMtx(rel_mtx);

		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];

			data.clear();

			data.vertices.resize(ai_mesh->mNumVertices);
			data.normals.resize(ai_mesh->mNumVertices);
			data.tangents.resize(ai_mesh->mNumVertices);
			data.uv.resize(ai_mesh->mNumVertices);
			data.vertex_colors.resize(ai_mesh->mNumVertices);

			for (unsigned int j = 0; j < ai_mesh->mNumVertices; j++) {

				data.vertices[j].x = ai_mesh->mVertices[j].x;
				data.vertices[j].y = ai_mesh->mVertices[j].y;
				data.vertices[j].z = ai_mesh->mVertices[j].z;

				if (ai_mesh->HasNormals())
				{
					data.normals[j].x = ai_mesh->mNormals[j].x;
					data.normals[j].y = ai_mesh->mNormals[j].y;
					data.normals[j].z = ai_mesh->mNormals[j].z;
				}


			
				if (ai_mesh->HasTextureCoords(0))
				{


					data.uv[j].x = ai_mesh->mTextureCoords[0][j].x;
					data.uv[j].y = ai_mesh->mTextureCoords[0][j].y;

					// tangent
					data.tangents[j].x = ai_mesh->mTangents[j].x;
					data.tangents[j].y = ai_mesh->mTangents[j].y;
					data.tangents[j].z = ai_mesh->mTangents[j].z;
				}

				if (ai_mesh->HasVertexColors(0)) {
					data.vertex_colors[j].x = ai_mesh->mColors[0][j].r;
					data.vertex_colors[j].y = ai_mesh->mColors[0][j].g;
					data.vertex_colors[j].z = ai_mesh->mColors[0][j].b;
					data.vertex_colors[j].w = ai_mesh->mColors[0][j].a;

				}

			}

			for (unsigned int j = 0; j < ai_mesh->mNumFaces; j++)
			{
				aiFace face = ai_mesh->mFaces[j];
				for (unsigned int k = 0; k < face.mNumIndices; k++)
					data.indices.push_back(face.mIndices[k]);
			}

			
			std::shared_ptr<Material> mat = nullptr;
			
			if(auto mat_manager = m_material_manager.lock()){
				mat = mat_manager->defaultMaterial();
				mat_manager->addMaterial(mat);
			}

			

			auto mesh = parent_ptr->addComponent<MeshComponent>(data);
			parent_ptr->addComponent<MeshRendererComponent>(mesh, mat);

			processMaterial(ai_mesh, scene, dir, mat);

		}

		if(node->mNumMeshes == 0){
			has_data = false;
		}

		for (int i = 0; i < node->mNumChildren; i++) {

			std::shared_ptr<Object> child_ptr = ObjectFactory::create();
			
			parent_ptr->addChild(child_ptr);
			processNode(node->mChildren[i], scene, dir, child_ptr);

		}
	}

void AssimpObjectLoader::processMaterial(aiMesh* mesh, const aiScene* scene, const STRING& dir, const std::shared_ptr<Material> obj_material) {

	if(scene->HasMaterials()) {
		if(mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			processTexture(material, scene, aiTextureType_DIFFUSE , dir, obj_material);
			processTexture(material, scene, aiTextureType_SPECULAR, dir, obj_material);
			processTexture(material, scene, aiTextureType_AMBIENT , dir, obj_material);
			processTexture(material, scene, aiTextureType_EMISSIVE, dir, obj_material);

			processTexture(material, scene, aiTextureType_NORMALS  , dir, obj_material);
			processTexture(material, scene, aiTextureType_OPACITY  , dir, obj_material);
			processTexture(material, scene, aiTextureType_SHININESS, dir, obj_material);

			processTexture(material, scene, aiTextureType_BASE_COLOR, dir, obj_material);
			processTexture(material, scene, aiTextureType_METALNESS , dir, obj_material);
			processTexture(material, scene, aiTextureType_EMISSION_COLOR   , dir, obj_material);
			processTexture(material, scene, aiTextureType_DIFFUSE_ROUGHNESS, dir, obj_material);
			processTexture(material, scene, aiTextureType_AMBIENT_OCCLUSION, dir, obj_material);

			processTexture(material, scene, aiTextureType_UNKNOWN, dir, obj_material);

		}

	}
}

void AssimpObjectLoader::processTexture(aiMaterial* material, const aiScene* scene, aiTextureType type, const STRING& dir,const std::shared_ptr<Material> obj_material){
	aiString path;

	if(material->GetTextureCount(type) > 0){
		//LOG_TRACE("Texture type: {0}, count: {1}", textureEnumToString(type), material->GetTextureCount(type));
	}else{
		return;
	}

	for(int i = 0; i < material->GetTextureCount(type); i++) {
		material->GetTexture(type, i, &path);
		const aiTexture* tex = scene->GetEmbeddedTexture(path.C_Str());

		if(nullptr == tex) {
			if(auto tex_manager = m_texture_manager.lock()) {
				std::shared_ptr<Texture> tex = nullptr;
				
				const STRING rel_path = dir + "/" + path.C_Str(); 
				tex = tex_manager->getTexture(rel_path.c_str());
				

				if(nullptr == tex){
					tex = tex_manager->createTexture(rel_path.c_str());
				}

				assignTextureToMaterial(type, tex, obj_material);
			}
		}
		else {
			//TODO: handle embedded texture

		}
	}


}

STRING AssimpObjectLoader::textureEnumToString(aiTextureType type){
	STRING str = "";

	switch(type) {
		case aiTextureType_DIFFUSE:
		{
			str = "DIFFUSE";
			break;
		}
		case aiTextureType_SPECULAR:
		{
			str = "SPECULAR";
			break;
		}
		case aiTextureType_AMBIENT:
		{
			str = "AMBIENT";
			break;
		}
		case aiTextureType_EMISSIVE:
		{
			str = "EMISSIVE";
			break;
		}
		case aiTextureType_NORMALS:
		{
			str = "NORMALS";
			break;
		}
		case aiTextureType_OPACITY:
		{
			str = "OPACITY";
			break;
		}
		case aiTextureType_SHININESS:
		{
			str = "SHININESS";
			break;
		}
		case aiTextureType_BASE_COLOR:
		{
			str = "BASE_COLOR";
			break;
		}
		case aiTextureType_METALNESS:
		{
			str = "METALNESS";
			break;
		}
		case aiTextureType_EMISSION_COLOR:
		{
			str = "EMISSION_COLOR";
			break;
		}
		case aiTextureType_DIFFUSE_ROUGHNESS:
		{
			str = "DIFFUSE_ROUGHNESS";
			break;
		}
		case aiTextureType_AMBIENT_OCCLUSION:
		{
			str = "AMBIENT_OCCLUSION";
			break;
		}
		case aiTextureType_UNKNOWN:
		{
			str = "UNKNOWN";
			break;
		}
	}

	return str;
}

void AssimpObjectLoader::assignTextureToMaterial(aiTextureType type, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Material>& obj_material){
	switch(type) {
		case aiTextureType_DIFFUSE:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_SPECULAR:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_AMBIENT:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_EMISSIVE:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_NORMALS:
		{
			obj_material->assignTexture(NORMAL_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_OPACITY:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_SHININESS:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_BASE_COLOR:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_METALNESS:
		{
			obj_material->assignTexture(METALLIC_UNIFROM_NAME, texture);
			break;
		}
		case aiTextureType_EMISSION_COLOR:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_DIFFUSE_ROUGHNESS:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_AMBIENT_OCCLUSION:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
		case aiTextureType_UNKNOWN:
		{
			obj_material->assignTexture(DIFFUSE_UNIFORM_NAME, texture);
			break;
		}
	}
}
	
	