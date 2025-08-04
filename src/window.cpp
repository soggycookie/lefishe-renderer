#include "window.h"

using namespace Lefishe;

Window::Window(const WindowConfig& config)
	: m_config(config)
{
}

bool Window::setup() {
	return create() && initOpenGLLoader();
}

bool Window::create() {
	/* Initialize the library */
	if (!glfwInit()) {
		DEBUG_ASSERT(1, "GLFW:: Failed to init!", ErrorLevel::FATAL)
		
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window_obj = glfwCreateWindow(m_config.screen_width, m_config.screen_height, m_config.name.data(), NULL, NULL);
	if (!m_window_obj)
	{
		DEBUG_ASSERT(1, "GLFW:: Failed to create Window!", ErrorLevel::FATAL)
		
		terminate();
		return false;
	}
	
	DEBUG_ASSERT(m_window_obj == nullptr, "GLFW:: Window Object is null!", ErrorLevel::FATAL)
	
	/* Make the window's context current */
	glfwMakeContextCurrent(m_window_obj);

	return true;
}

bool Window::initOpenGLLoader() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// Handle initialization failure
		DEBUG_ASSERT(1, "GLAD:: Failed to load OpenGL!", ErrorLevel::FATAL)
		
		return false;
	}

	return true;
}

void Window::update() {
	while (!glfwWindowShouldClose(m_window_obj))
	{
		//TODO:
		// 
		//OnStart.broadcast(); 
		//OnUpdate.broadcast();

		//render
		//render queue draw

		swapBuffer();
		pollEvents();
	}
}

void Window::swapBuffer() {
	glfwSwapBuffers(m_window_obj);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::terminate() {
	glfwTerminate();
}



void Window::shutdown() {
	terminate();
}