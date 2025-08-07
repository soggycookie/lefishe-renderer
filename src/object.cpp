#include "object.h"
#include "component.h"
#include "id_generator.h"

using namespace Lefishe;

Object::Object()
	: m_id(IDGenerator::Generate())
{
}

void Object::addComponent(BaseComponent& component){
	if(m_components.contains(component.id())){
		LOG_INFO("Object can't add duplicated component!");
		return;
	}
	m_components.insert({component.id(), component});

}

void Object::addComponent(BaseComponent&& component){
	if(m_components.contains(component.id())){
		LOG_INFO("Object can't add duplicated component!");
		return;
	}
	m_components.insert({component.id(), std::move(component)});

}

void Object::addChild(std::shared_ptr<Object> obj){
	if(m_children.contains(obj->id())){
		LOG_INFO("Object can't add duplicated child!");
		return;
	}
	m_children.insert({obj->id(), obj});
}

const std::map<ObjectID, std::shared_ptr<Object>>& Object::children() const{
	return m_children;
}

const std::map<ComponenetID, BaseComponent>& Object::components() const{
	return m_components;
}

long Object::id() const{
	return m_id;
}