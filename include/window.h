#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "global_header.h"
#include "renderer.h"


namespace Lefishe {

	using WindowObject = GLFWwindow;

	struct WindowConfig {
		UINT screen_width = 1280;
		UINT screen_height = 720;
		STRINGVIEW name = "Lefishe Renderer";
	};

	struct FrameData {
		DOUBLE delta_time;
		DOUBLE current_time;
		IVEC2  pixel_curser_pos;
		VEC2   curser_pos;
	};

	class Window {
	public:
		Window(const WindowConfig& config);
		Window() = default;

		bool setup();
		void update();
		void shutdown();

	private:
		bool create();
		bool initOpenGLLoader();
		//void initImGui();
		//void newFrameImGui();
		//void terminateImGui();
		void swapBuffer();
		void pollEvents();
		void terminate();


	private:
		WindowObject* m_window_obj;
		WindowConfig m_config;
		FrameData m_framedata;

		Renderer m_renderer;
	};

}