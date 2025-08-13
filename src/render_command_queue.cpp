#include "render_command_queue.h"

using namespace Lefishe;



void RenderCommandQueue::submit(const std::unordered_map<ObjectID, std::shared_ptr<Object>>& data){
	
	//Buffer indirect_buf(sizeof(DrawElementIndirectCommand));

	for(const auto& [key, obj] : data) {
		if(auto mesh_renderer = obj->getComponent<MeshRendererComponent>()){
			auto mesh = mesh_renderer->mesh();
		
			Buffer vert_buffer(mesh->vertexDataByteSize());
			vert_buffer.subData(mesh->vertexByteSize(), mesh->vertexByteOffset(), mesh->vertices().data());
			vert_buffer.subData(mesh->normalByteSize(), mesh->normalByteOffset(), mesh->normals().data());
			vert_buffer.subData(mesh->tangentByteSize(), mesh->tangentByteOffset(), mesh->tangents().data());
			vert_buffer.subData(mesh->vertexColorByteSize(), mesh->vertexColorByteOffset(), mesh->vertexColors().data());
			vert_buffer.subData(mesh->uvByteSize(), mesh->uvByteOffset(), mesh->uv().data());
			Buffer element_buffer(mesh->indexByteSize(), mesh->indices().data());
		
			m_drawcalls.emplace_back(vert_buffer, element_buffer ,mesh_renderer , 0);
		
			auto t = obj->transform();

			mesh_renderer->material().setUniformData("model", &(t->localToWorldMtx()));
		
		}


		//DrawElementIndirectCommand cmd{
		//	.count = mesh->indexSize(),
		//	.instance_count = 1,
		//	.first_index = 0,
		//	.base_vertex = 0,
		//	.base_instance = 0
		//};

		//Buffer indirect_buf(sizeof(DrawElementIndirectCommand), &cmd);
		//indirect_buf.bind(INDIRECT);


	}

	//indirect_buf.bind(INDIRECT);
}

const std::vector<DrawCall>& RenderCommandQueue::drawcall() const{
	return m_drawcalls;
}