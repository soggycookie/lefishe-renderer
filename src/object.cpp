#include "object.h"
#include "component.h"
#include "id_generator.h"

using namespace Lefishe;

Object::Object()
	: m_id(IDGenerator::Generate())
{
}

void Object::addComponent(const Component& component){

}

void Object::addChild(const Object& obj){
	m_children.insert({obj.getID(), obj});
}

void Object::addChild(Object&& obj){
	m_children.insert({obj.getID(), std::move(obj)});
}

//void Object::addComponents(){
//
//}
//
//void Object::addChildren(){
//
//}

const std::map<ObjectID, Object>& Object::children() const{
	return m_children;
}

const std::map<ComponenetID, Component>& Object::components() const{
	return m_components;
}

long Object::getID() const{
	return m_id;
}