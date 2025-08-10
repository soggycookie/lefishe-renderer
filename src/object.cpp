#include "object.h"
#include "component.h"
#include "id_generator.h"

using namespace Lefishe;

Object::Object()
	: m_id(IDGenerator::Generate())
{
}

void Object::addChild(std::shared_ptr<Object> obj){
	if(m_children.contains(obj->id())){
		LOG_INFO("Object can't add duplicated child!");
		return;
	}
	m_num_children++;
	m_children.insert({obj->id(), obj});
	obj->parent(shared_from_this());
	
	if(!rootParent()){
		obj->rootParent(shared_from_this());
	}

}

const std::unordered_map<ObjectID, std::shared_ptr<Object>>& Object::children() const{
	return m_children;
}

const std::unordered_map<ComponentID, std::shared_ptr<BaseComponent>>& Object::components() const{
	return m_components;
}

std::shared_ptr<Object> Object::parent() const{
	return m_parent.lock();
}

std::shared_ptr<Object> Object::rootParent() const{
	return m_root_parent.lock();
}

bool Object::haveComponent(ComponentID id) const{
	return m_components.contains(id);
}

bool Object::haveComponentInChildren(ComponentID id) const{
	for(const auto& child : m_children){
		if(child.second->haveComponent(id) || 
		   child.second->haveComponentInChildren(id))
		{
			return true;
		}	
	}

	return false;
}


void Object::parent(std::weak_ptr<Object> parent){
	m_parent = parent;
}

void Object::rootParent(std::weak_ptr<Object> root_parent){
	m_root_parent = root_parent;
}

long Object::id() const{
	return m_id;
}

std::shared_ptr<Object> ObjectFactory::create(){
	auto obj = std::make_shared<Object>();
	obj->addComponent<TransformComponent>();

	return obj;
}

void Object::updateComponents(){
	for(auto& component: m_components){
		component.second->update();
	}

	for(auto& child : m_children){
		child.second->updateComponents();
	}
}



