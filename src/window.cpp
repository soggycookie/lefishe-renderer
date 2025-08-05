#include "window.h"

using namespace Lefishe;

Window::Window(const WindowConfig& config)
	: m_config(config)
{
}

BOOL Window::setup() {
	return create() && initOpenGLLoader();
}

BOOL Window::create() {
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
	glfwSwapInterval(1);
	return true;
}

BOOL Window::initOpenGLLoader() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// Handle initialization failure
		DEBUG_ASSERT(1, "GLAD:: Failed to load OpenGL!", ErrorLevel::FATAL)
		
		return false;
	}

	return true;
}

void Window::config() {

}

void Window::update() {
	while (!glfwWindowShouldClose(m_window_obj))
	{
		setCurrentTime();
		setDeltaTime();

		//TODO:
		// 
		//OnStart.broadcast(); 
		//OnUpdate.broadcast();
		

		
		//render
		//render queue draw

		swapBuffer();
		pollEvents();

		setLastFrameTime();
	}
}

void Window::swapBuffer() const {
	glfwSwapBuffers(m_window_obj);
}

void Window::pollEvents() const {
	glfwPollEvents();
}

void Window::terminate() const {
	glfwTerminate();
}

void Window::shutdown() const {
	terminate();
}

WindowObject* Window::get(){
	return m_window_obj;
}

void Window::setDeltaTime() {
	m_framedata.delta_time = m_framedata.current_time - m_framedata.m_last_frame_time;
}

void Window::setCurrentTime() {
	m_framedata.current_time = glfwGetTime();
}

void Window::setLastFrameTime() {
	m_framedata.m_last_frame_time = m_framedata.current_time;
}