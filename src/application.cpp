#include "application.h"

Application::Application() {
	m_window = std::make_unique<Lefishe::Window>();
}

void Application::run() {
	if (m_window->setup()) {
		DEBUG_ASSERT(1, "Application:: Application started!", ErrorLevel::INFO)
		m_window->update();
		m_window->shutdown();
	}
	else {
		DEBUG_ASSERT(1, "Application:: Application failed to set up!", ErrorLevel::FATAL)
	}
	
}