#include "scene.h"

using namespace Lefishe;



void Scene::addObject(std::shared_ptr<Object>&& obj){
	auto o = std::move(obj);

	m_all_objs.insert({o->id(), o});
	
	fetch(o);
}

void Scene::fetch(const std::shared_ptr<Object>& obj){
	updateObject(obj);

	if(obj->haveComponent<MeshRendererComponent>())
	{
		m_render_objs.insert({obj->id(), obj->getComponent<MeshRendererComponent>()});	
	}

	if(obj->haveComponent<CameraComponent>())
	{

		auto c = obj->getComponent<CameraComponent>();
		auto t = obj->getComponent<TransformComponent>();
		
		m_camera_objs.insert({obj->id(), c});
		
		if(CameraComponent::main() ==  c){
			m_cam = obj;
			
			MAT4 cam_ubo_data[2] = {
				t->worldToLocalMtx(),
				c->perspectiveMtx()
			};

			m_cam_ubo = Buffer(sizeof(cam_ubo_data), cam_ubo_data);
			m_cam_ubo.bindIndex(UNIFORM, 0);
		}
	}

	for(auto& [key, child] : obj->children()){
		fetch(child);
	}

}

const std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>>& Scene::renderObjects() const{
	return m_render_objs;
}

const std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>>& Scene::cameraObjects() const{
	return m_camera_objs;
}

std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>>& Scene::renderObjects() {
	return m_render_objs;
}

std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>>& Scene::cameraObjects() {
	return m_camera_objs;
}

const std::shared_ptr<Object>& Scene::cameraObject() const{
	return m_cam;
}


void Scene::update(){
	for(auto& obj : m_all_objs){
		updateObject(obj.second);
	}
}

void Scene::updateObject(const std::shared_ptr<Object>& obj){
	for(const auto& component : obj->components()) {
		component.second->update();
	}

	for(const auto& child : obj->children()) {
		updateObject(child.second);
	}
}



