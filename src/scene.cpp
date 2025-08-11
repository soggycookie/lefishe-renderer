#include "scene.h"

using namespace Lefishe;

void Scene::addObject(std::shared_ptr<Object> obj){
	auto o = obj;
	if(o->haveComponent<MeshRendererComponent>() || 
	   o->haveComponentInChildren<MeshRendererComponent>())
	{
		m_render_objs.insert({o->id(), o});	
			
		LOG_TRACE("Mesh renderer is here");
	}

	if(o->haveComponent<CameraComponent>() || 
	   o->haveComponentInChildren<CameraComponent>())
	{
		m_camera_objs.insert({o->id(), o});
		auto c = o->getComponent<CameraComponent>();
		if(CameraComponent::main() ==  c){
			m_cam = o;
			LOG_TRACE("Main camera here!");
		}
	}

	m_all_objs.insert({o->id(), o});
}

const std::unordered_map<ObjectID, std::shared_ptr<Object>>& Scene::renderObjects() const{
	return m_render_objs;
}

const std::unordered_map<ObjectID, std::shared_ptr<Object>>& Scene::cameraObjects() const{
	return m_camera_objs;
}

const std::shared_ptr<Object>& Scene::cameraObject() const{
	return m_cam;
}

