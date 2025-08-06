#include "input_listener.h"

using namespace Lefishe;

InputListener::InputListener(WindowObject* obj)
	: m_window_object(obj)
{ 
	std::fill(std::begin(m_key_state), std::end(m_key_state), false);

	if (obj != nullptr) {
		setCallback();
	}
}

InputListener& InputListener::operator=(InputListener&& input) noexcept{
	 
	if (this != &input) {
		delete m_window_object;
		m_window_object = input.m_window_object;

		input.m_window_object = nullptr;

		setCallback();
	}
	else {
		//DEBUG_ASSERT(1, "Input:: Self move assignment!", ErrorLevel::WARNING);
	}

	return *this;
}

bool InputListener::isMousePressed(MouseButton mouse) {
	return m_mouse_state[static_cast<int>(mouse)];
}

bool InputListener::isMouseReleased(MouseButton mouse) {
	return !isMousePressed(mouse);
}

bool InputListener::isKeyPressed(KeyButton key) {
	return m_key_state[static_cast<int>(key)];
}

bool InputListener::isKeyReleased(KeyButton key) {
	return !isKeyPressed(key);
}

void InputListener::keyButtonCallback(WindowObject* window, int key, int scancode, int action, int mods) {
	InputListener* instance = static_cast<InputListener*>(glfwGetWindowUserPointer(window));
	
	if (instance == nullptr) {
		LOG_WARN("[INPUT] Key callback can't access instance's pointer!");
		return;
	}

	if (action == GLFW_PRESS) {
		instance->m_key_state[key] = true;
	}
	else if(action == GLFW_RELEASE) {
		instance->m_key_state[key] = false;
	}
}

void InputListener::mouseButtonCallback(WindowObject* window, int button, int action, int mods) {
	InputListener* instance = static_cast<InputListener*>(glfwGetWindowUserPointer(window));

	if (instance == nullptr) {
		LOG_WARN( "[INPUT] Mouse callback can't access instance's pointer!");
		return;
	}

	int index = GLFW_MOUSE_BUTTON_LEFT ? (GLFW_MOUSE_BUTTON_RIGHT ? 1 : 2) : 0;

	if (action == GLFW_PRESS) {
		instance->m_mouse_state[index] = true;
	}
	else if (action == GLFW_RELEASE) {
		instance->m_mouse_state[index] = false;
	}
}

void InputListener::cursorPosCallback(WindowObject* window, double xpos, double ypos) {

	InputListener* instance = static_cast<InputListener*>(glfwGetWindowUserPointer(window));

	if (instance == nullptr) {
		LOG_WARN( "[INPUT] Mouse callback can't access instance's pointer!");
		return;
	}

	int x_pos_i = static_cast<int>(xpos);
	int y_pos_i = static_cast<int>(ypos);

	instance->m_cursor_pos = IVEC2(x_pos_i, y_pos_i);
	instance->m_mouse_delta = instance->m_last_cursor_pos - IVEC2(x_pos_i, y_pos_i);
	instance->m_last_cursor_pos = instance->m_cursor_pos;
}

void InputListener::setCallback() {
	glfwSetWindowUserPointer(m_window_object, this);
	glfwSetKeyCallback(m_window_object, keyButtonCallback);
	glfwSetMouseButtonCallback(m_window_object, mouseButtonCallback);
	glfwSetCursorPosCallback(m_window_object, cursorPosCallback);
}