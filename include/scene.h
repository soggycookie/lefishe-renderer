#pragma once
#include "global_header.h"
#include "object.h"

namespace Lefishe {
	

	class Scene {
	public:

		void addObject(std::shared_ptr<Object> obj);

		const std::unordered_map<ObjectID, std::shared_ptr<Object>>& renderObjects() const;
		const std::unordered_map<ObjectID, std::shared_ptr<Object>>& cameraObjects() const;
		const std::shared_ptr<Object>& cameraObject() const;

		void update(){
			for(auto& obj : m_all_objs){
				obj.second->updateComponents();
			}
		}


	private:


	private:
		std::unordered_map<ObjectID, std::shared_ptr<Object>> m_all_objs;
		std::unordered_map<ObjectID, std::shared_ptr<Object>> m_render_objs;
		std::unordered_map<ObjectID, std::shared_ptr<Object>> m_camera_objs;
		std::shared_ptr<Object> m_cam;
	};

}
