#pragma once
#include "global_header.h"
#include "component.h"

namespace Lefishe {

	using ObjectID = long;
	using ComponentID = std::type_index;

	class Object : public std::enable_shared_from_this<Object> {
	public:
		friend class TransformComponent;

		Object();


		template<typename T>
		std::shared_ptr<T> getComponent(){
			std::type_index id = std::type_index(typeid(T));
			
			if(m_components.contains(id)){
				return std::dynamic_pointer_cast<T>(m_components.at(id));
			}
			LOG_WARN("This object ID {0} does not have {1}!", m_id, id.name());
			return nullptr;
		}

		template<typename T, typename... Args>
		void addComponent(Args... args){

			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");
			
			std::type_index id = std::type_index(typeid(T));
			
			if(m_components.contains(id)){
				LOG_WARN("{0} is not concrete class of BaseComponent!", id.name());
			}else{
				//LOG_TRACE("Added component of type {0}", id.name());
				m_components.insert({id, std::make_shared<T>(std::forward<Args>(args)...)});
			}
		}

		template<typename T>
		void copyComponent(const std::shared_ptr<T>& component){

			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");
			
			std::type_index id = std::type_index(typeid(T));
			
			if(m_components.contains(id)){
				// Update existing component
				if (auto existing = std::dynamic_pointer_cast<T>(m_components.at(id))) {
					*existing = *component; // Use assignment operator for deep copy
					//LOG_TRACE("Updated existing component of type {0}", id.name());
				} else {
					LOG_WARN("Failed to cast existing component of type {0} to T", id.name());
				}
			}else{
				LOG_WARN("Object does not contain {0}", id.name());

				return;
			}
		}


		template<typename T>
		void moveComponent(std::shared_ptr<T>& component) {
			static_assert(std::is_base_of<BaseComponent, T>::value, "Derived must be derived from BaseComponent");

			std::type_index id = std::type_index(typeid(T));
    
			if (m_components.contains(id)) {
				// Update existing component
				if (auto existing = std::dynamic_pointer_cast<T>(m_components.at(id))) {
					existing = std::move(component);
					//LOG_TRACE("Updated existing component of type {0}", id.name());
				} else {
					LOG_WARN("Failed to cast existing component of type {0} to T", id.name());
				}
			}

            m_components.insert({id, std::move(component)});
		}

		
        template<typename T>
		bool haveComponent() const{
			return m_components.contains(std::type_index(typeid(T)));
		}

		template<typename T>
		bool haveComponentInChildren() const{
			std::type_index id = std::type_index(typeid(T));
			return haveComponentInChildren(id);
		}

		
		void init();

		void addChild(const std::shared_ptr<Object>& obj);

		const std::unordered_map<ObjectID, std::shared_ptr<Object>>& children() const;
		const std::unordered_map<ComponentID, std::shared_ptr<BaseComponent>>& components() const;

		std::shared_ptr<Object> parent() const;
		std::shared_ptr<Object> rootParent() const;
		std::shared_ptr<TransformComponent> transform() const;

		void parent(std::weak_ptr<Object> parent);
		void rootParent(std::weak_ptr<Object> root_parent);

		UINT numChildren() const;

		long id() const;

		void copyObject(const Object& obj);

		//to avoid shared_ptr's content re-assigning
		Object& operator=(const Object& obj) = delete;
		Object& operator=(Object&& obj)      = delete;

	private:
		bool haveComponent(ComponentID id) const;
		bool haveComponentInChildren(ComponentID id) const;
		void onTransformChanged();


	private:


		long m_id;
		UINT m_num_children = 0;
		std::weak_ptr<Object> m_parent;
		std::weak_ptr<Object> m_root_parent;
		std::weak_ptr<TransformComponent> m_transform;

		std::unordered_map<ObjectID, std::shared_ptr<Object>> m_children;
		std::unordered_map<ComponentID, std::shared_ptr<BaseComponent>> m_components;
	};

	class ObjectFactory{
	public:
		static std::shared_ptr<Object> create();
	};
}