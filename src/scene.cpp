#include "scene.h"

using namespace Lefishe;

void Scene::addObject(const Object& obj){
	m_objects.push_back(obj);
}

void Scene::addObject(Object&& obj){
	m_objects.push_back(std::move(obj));
}

void Scene::addObjects(const std::vector<Object>& objs){
	m_objects.insert(m_objects.end(), objs.begin(), objs.end());
}

void Scene::addObjects(std::vector<Object>&& objs){
	m_objects.insert(
		m_objects.end(),
		std::make_move_iterator(objs.begin()),
		std::make_move_iterator(objs.end())
	);
}