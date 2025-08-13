#pragma once
#include "global_header.h"
#include "draw_call.h"
#include "object.h"
#include "renderer.h"

namespace Lefishe{
	
	class Object;

	class RenderCommandQueue{
	public:
		RenderCommandQueue() = default;

		void submit(const std::unordered_map<ObjectID, std::shared_ptr<Object>>& data);
		const std::vector<DrawCall>& drawcall() const;

	private:
		//void process();

	private:
		std::vector<DrawCall> m_drawcalls;
	};

}