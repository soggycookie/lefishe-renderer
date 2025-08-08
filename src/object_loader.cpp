#include "object_loader.h"
#include "object.h"

using namespace Lefishe;

	std::shared_ptr<Object> AssimpObjectLoader::loadObject(const std::string& path) {
		
		const aiScene* scene = m_importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		if (scene == nullptr) {
			LOG_WARN("[ASSIMP] Scene is null! Check the asset path!");
			return nullptr;
		}

		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			LOG_WARN("[ASSIMP] {0}", m_importer.GetErrorString());
			return nullptr;
		}

		if (!scene->mRootNode) {
			LOG_WARN("[ASSIMP] Scene is empty!");
			return nullptr;
		}

		std::shared_ptr<Object> parent_ptr = ObjectFactory::create();

		std::string directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene, parent_ptr);

		return parent_ptr;
	}

	void AssimpObjectLoader::processNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Object> parent_ptr) {

		MeshData data;
		bool has_data = true;;

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

				if (ai_mesh->mTextureCoords[0])
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

			
			parent_ptr->addMoveComponent(std::make_shared<MeshComponent>(std::move(data)));
		
			aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
		}

		if(node->mNumMeshes == 0){
			has_data = false;
		}

		for (int i = 0; i < node->mNumChildren; i++) {
			if(has_data){
				std::shared_ptr<Object> child_ptr = ObjectFactory::create();
				parent_ptr->addChild(child_ptr);
				processNode(node->mChildren[i], scene, child_ptr);
			}else{
				processNode(node->mChildren[i], scene, parent_ptr);
			}
		}
	}