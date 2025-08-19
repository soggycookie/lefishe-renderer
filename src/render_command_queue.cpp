#include "render_command_queue.h"

using namespace Lefishe;

RenderCommandQueue::RenderCommandQueue(){

}

void RenderCommandQueue::submitAndDraw(std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>>& cam_map, std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>>& mesh_renderer_map){
	
	if(m_drawcalls.size() < mesh_renderer_map.size()){
		m_drawcalls.resize(mesh_renderer_map.size());
	}
	
	for(auto& [key, camera_ptr] : cam_map) {
		int index = 0;
		for(auto& data : mesh_renderer_map) {
			auto mesh_renderer = data.second.lock();
			float dist = 
				glm::length(
					mesh_renderer->owner()->transform()->position() - 
					camera_ptr.lock()->owner()->transform()->position()
				);

			CommandBucket bucket(dist, mesh_renderer->material());

			if(!mesh_renderer->hasDrawcall()) {
				createDrawCall(data);
			}
			m_drawcalls[index] = std::make_pair(bucket, mesh_renderer->drawcall());

			index++;
		}
		sort();

		auto camera = camera_ptr.lock();

		camera->startRender();

		//LOG_TRACE("{0}", m_drawcalls.size());

		for(int i = 0 ; i < m_drawcalls.size();i++){
			m_renderer.draw(*(m_drawcalls[i].second.lock()));
		}
	}

	//Buffer indirect_buf(sizeof(DrawElementIndirectCommand));
			//DrawElementIndirectCommand cmd{
		//	.count = mesh->indexSize(),
		//	.instance_count = 1,
		//	.first_index = 0,
		//	.base_vertex = 0,
		//	.base_instance = 0
		//};

		//Buffer indirect_buf(sizeof(DrawElementIndirectCommand), &cmd);
		//indirect_buf.bind(INDIRECT);
	//indirect_buf.bind(INDIRECT);
}



//std::unordered_map<ObjectID, DrawCall>& RenderCommandQueue::drawcall(){
//	return m_drawcalls;
//}



void RenderCommandQueue::sort(){
	std::sort(m_drawcalls.begin(), m_drawcalls.end(),
		[](const auto& a, const auto& b){
			const CommandBucket bucket_a = a.first;
			const CommandBucket bucket_b = b.first;

			GeometryType mat_a_type = bucket_a.material.lock()->materialSettings().geo_type;
			GeometryType mat_b_type = bucket_b.material.lock()->materialSettings().geo_type;

			if(mat_a_type == GeometryType::TRANSPARENT_GEO && mat_b_type == GeometryType::OPAQUE_GEO )
			{
				return false;
			}else if (mat_b_type == GeometryType::TRANSPARENT_GEO && mat_a_type == GeometryType::OPAQUE_GEO )
			{
				return true;
			}else{
				if(mat_a_type == GeometryType::OPAQUE_GEO){
					return bucket_a.distance < bucket_b.distance;
				}else{
					return bucket_a.distance > bucket_b.distance;
				}
			}
		}
	);
}

void RenderCommandQueue::createDrawCall(const std::pair<ObjectID, std::weak_ptr<MeshRendererComponent>>& data){
	auto mesh_renderer = data.second.lock();
	auto mesh = mesh_renderer->mesh();

	Buffer vert_buffer(mesh->vertexDataByteSize());
	vert_buffer.subData(mesh->vertexByteSize(), mesh->vertexByteOffset(), mesh->vertices().data());
	vert_buffer.subData(mesh->normalByteSize(), mesh->normalByteOffset(), mesh->normals().data());
	vert_buffer.subData(mesh->tangentByteSize(), mesh->tangentByteOffset(), mesh->tangents().data());
	vert_buffer.subData(mesh->vertexColorByteSize(), mesh->vertexColorByteOffset(), mesh->vertexColors().data());
	vert_buffer.subData(mesh->uvByteSize(), mesh->uvByteOffset(), mesh->uv().data());
	Buffer element_buffer(mesh->indexByteSize(), mesh->indices().data());

	auto dc = std::make_shared<DrawCall>(vert_buffer, element_buffer, *mesh_renderer, 0);
	mesh_renderer->drawcall(dc);
}

