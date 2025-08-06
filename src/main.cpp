#include "application.h"
#include "logger.h"

int main() {
	LOG_INIT();

	Application* app = new Application;
	app->run();

	delete app;
}