#include "application.h"


Application::Application() {
	m_window = std::make_unique<Lefishe::Window>();

}

void Application::run() {
	if (m_window->setup()) {
		LOG_INFO("Application started!");

		m_window->update();
		m_window->shutdown();
	}
	else {
		LOG_ERROR("Application:: Application failed to set up!");
	}
	
}