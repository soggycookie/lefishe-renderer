#pragma once
#include "global_header.h"
#include "object.h"

namespace Lefishe {
	

	class Scene {
	public:

		void addObject(const Object& obj);
		void addObject(Object&& obj);

		void addObjects(const std::vector<Object>& objs);
		void addObjects(std::vector<Object>&& objs);

	private:
		std::vector<Object> m_objects;
	};

	
}
