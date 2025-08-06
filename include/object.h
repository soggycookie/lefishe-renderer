#pragma once
#include "global_header.h"


namespace Lefishe {

	using ObjectID = long;
	using ComponenetID = int;

	class Component;

	class Object {
	public:
		Object();

		void addComponent(const Component& component);
		void addChild(const Object& obj);
		void addChild(Object&& obj);

		//void addComponents();
		//void addChildren();

		const std::map<ObjectID, Object>& children() const;
		const std::map<ComponenetID, Component>& components() const;

		long getID() const;

	private:
		long m_id;

		std::map<ObjectID, Object> m_children;
		std::map<ComponenetID, Component> m_components;
	};

}