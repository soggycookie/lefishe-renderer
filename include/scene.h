#pragma once
#include "global_header.h"
#include "object.h"
#include "buffer.h"

namespace Lefishe {
	

	class Scene {
	public:
		void addObject(std::shared_ptr<Object>&& obj);

		const std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>>& renderObjects() const;
		const std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>>& cameraObjects() const;

		std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>>& renderObjects();
		std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>>& cameraObjects() ;

		const std::shared_ptr<Object>& cameraObject() const;

		void update();


	private:
		void fetch(const std::shared_ptr<Object>& obj);
		void updateObject(const std::shared_ptr<Object>& obj);

	private:
		std::unordered_map<ObjectID, std::shared_ptr<Object>> m_all_objs;
		std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>> m_render_objs;
		std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>> m_camera_objs;
		std::shared_ptr<Object> m_cam;

		Buffer m_cam_ubo;
	};

}
