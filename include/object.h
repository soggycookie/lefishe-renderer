#pragma once
#include "global_header.h"
#include "component.h"

namespace Lefishe {

	using ObjectID = long;
	using ComponentID = std::type_index;

	class Object : public std::enable_shared_from_this<Object> {
	public:
		Object();

		template<typename T>
		std::shared_ptr<T> getComponent(){
			std::type_index id = std::type_index(typeid(T));
			return std::dynamic_pointer_cast<T>(m_components.at(id));
		}

		template<typename T>
		void addComponent(){

			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");
			
			std::type_index id = std::type_index(typeid(T));
			
			if(m_components.contains(id)){
				LOG_WARN("{0} is not concrete class of BaseComponent!", id.name());
			}else{
				//LOG_TRACE("Added component of type {0}", id.name());
				m_components.insert({id, std::make_shared<T>()});
			}
		}

		template<typename T>
		void copyComponent(std::shared_ptr<T> obj){

			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");
			
			std::type_index id = std::type_index(typeid(T));
			
			if(m_components.contains(id)){
				// Update existing component
				if (auto existing = std::dynamic_pointer_cast<T>(m_components.at(id))) {
					*existing = *obj; // Use assignment operator for deep copy
					//LOG_TRACE("Updated existing component of type {0}", id.name());
				} else {
					LOG_WARN("Failed to cast existing component of type {0} to T", id.name());
				}
			}else{
				LOG_WARN("Object does not contain {0}", id.name());
			}
		}

		template<typename T>
		void moveComponent(std::shared_ptr<T>&& obj) {
			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");

			std::type_index id = std::type_index(typeid(T));
    
			if (m_components.contains(id)) {
				LOG_WARN("Component type {0} already exists, skipping move!", id.name());
				return;
			}

			m_components.insert({id, std::move(obj)});
		}

		template<typename T>
		void addMoveComponent(std::shared_ptr<T>&& obj) {
			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");

			std::type_index id = std::type_index(typeid(T));
    
			if (m_components.contains(id)) {
				// Update existing component
				if (auto existing = std::dynamic_pointer_cast<T>(m_components.at(id))) {
					existing = std::move(obj);
					//LOG_TRACE("Updated existing component of type {0}", id.name());
				} else {
					LOG_WARN("Failed to cast existing component of type {0} to T", id.name());
				}
			}else{
				//LOG_TRACE("Added and moved component of type {0}", id.name());
				m_components.insert({id, std::move(obj)});
			}

		}

		
		template<typename T>
		bool haveComponent() const{
			return m_components.contains(std::type_index(typeid(T)));
		}

		template<typename T>
		bool haveComponentInChildren() const{
			std::type_index id = std::type_index(typeid(T));
			for(const auto& child : m_children){
				if(child.second->haveComponent(id) || 
				   child.second->haveComponentInChildren(id))
				{
					return true;
				}	
			}

			return false;
		}



		void addChild(std::shared_ptr<Object> obj);

		const std::unordered_map<ObjectID, std::shared_ptr<Object>>& children() const;
		const std::unordered_map<ComponentID, std::shared_ptr<BaseComponent>>& components() const;

		std::shared_ptr<Object> parent() const;
		std::shared_ptr<Object> rootParent() const;

		void parent(std::weak_ptr<Object> parent);
		void rootParent(std::weak_ptr<Object> root_parent);

		long id() const;

	private:
		bool haveComponent(ComponentID id) const;
		bool haveComponentInChildren(ComponentID id) const;


	private:


		long m_id;
		int m_num_children = 0;
		std::weak_ptr<Object> m_parent;
		std::weak_ptr<Object> m_root_parent;


		std::unordered_map<ObjectID, std::shared_ptr<Object>> m_children;
		std::unordered_map<ComponentID, std::shared_ptr<BaseComponent>> m_components;
	};

	class ObjectFactory{
	public:
		static std::shared_ptr<Object> create();
	};
}