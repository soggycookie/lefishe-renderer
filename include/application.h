#pragma once
#include "window.h"
#include "logger.h"

class Application {
public:
	Application();
	

	void run();

private:
	std::unique_ptr<Lefishe::Window> m_window;

};