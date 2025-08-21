#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "global_header.h"
#include "input_listener.h"
#include "logger.h"

#include "event_bus.h"

namespace Lefishe {

	using WindowObject = GLFWwindow;

	struct WindowConfig {
		UINT screen_width = 1280;
		UINT screen_height = 720;
		STRINGVIEW name = "Lefishe Renderer";
		bool is_full_screen = false;
	};

	struct FrameData {
		DOUBLE delta_time        = 0.0;
		DOUBLE current_time      = 0.0;
		DOUBLE m_last_frame_time = 0.0;
	};

	class Window {
	public:
		Window(const WindowConfig& config);
		Window() = default;

		bool setup();
		void update();
		void shutdown() const;

		WindowObject* get();

	private:
		bool create();
		bool initOpenGLLoader();

		void config();

		//void initImGui();
		//void newFrameImGui();
		//void terminateImGui();
		void swapBuffer() const;
		void pollEvents() const;
		void terminate() const;

		void setDeltaTime();
		void setCurrentTime();
		void setLastFrameTime();

	private:
		WindowObject* m_window_obj;
		WindowConfig m_config;
		FrameData m_framedata;
		//Renderer m_renderer;
		//EventBus<> m_on_start;


	};

}