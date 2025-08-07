#pragma once
#include "global_header.h"
#include "component.h"

namespace Lefishe {

	using ObjectID = long;
	using ComponenetID = int;

	class Object {
	public:
		Object();

		void addComponent(BaseComponent& component);
		void addComponent(BaseComponent&& component);
		void addChild(std::shared_ptr<Object> obj);

		const std::map<ObjectID, std::shared_ptr<Object>>& children() const;
		const std::map<ComponenetID, BaseComponent>& components() const;

		long id() const;

	private:
		long m_id;

		std::map<ObjectID, std::shared_ptr<Object>> m_children;
		std::map<ComponenetID, BaseComponent> m_components;
	};

}