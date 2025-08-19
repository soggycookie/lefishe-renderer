#pragma once
#include "global_header.h"
#include "draw_call.h"
#include "object.h"
#include "renderer.h"
#include "material.h"

namespace Lefishe{
	
	struct CommandBucket{
		FLOAT distance = 0.0;
		std::weak_ptr<Material> material;
	};

	class Object;

	class RenderCommandQueue{
	public:
		RenderCommandQueue();

		void submitAndDraw(std::unordered_map<ObjectID, std::weak_ptr<CameraComponent>>& cam_map, std::unordered_map<ObjectID, std::weak_ptr<MeshRendererComponent>>& mesh_renderer_map);

	private:
		void sort();
		void createDrawCall(const std::pair<ObjectID, std::weak_ptr<MeshRendererComponent>>& data);

		//std::unordered_map<ObjectID, DrawCall>& drawcall();

	private:
		std::vector<std::pair<CommandBucket, std::weak_ptr<DrawCall>>> m_drawcalls;


		Renderer m_renderer;
		std::vector<Buffer> m_cam_ubos;
	};

}