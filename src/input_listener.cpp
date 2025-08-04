#include "input_listener.h"

using namespace Lefishe;

InputListener::InputListener(const WindowObject* obj)
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
		DEBUG_ASSERT(1, "Input:: Self move assignment!", ErrorLevel::WARNING);
	}

	return *this;
}

BOOL InputListener::isMousePressed(MouseButton mouse) {
	return m_mouse_state[static_cast<int>(mouse)];
}

BOOL InputListener::isMouseReleased(MouseButton mouse) {
	return !isMousePressed(mouse);
}

BOOL InputListener::isKeyPressed(KeyButton key) {
	return m_key_state[static_cast<int>(key)];
}

BOOL InputListener::isKeyReleased(KeyButton key) {
	return !isKeyPressed(key);
}

void InputListener::keyButtonCallback(WindowObject* window, int key, int scancode, int action, int mods) {
	InputListener* instance = static_cast<InputListener*>(glfwGetWindowUserPointer(window));
	
	if (instance == nullptr) {
		DEBUG_ASSERT(1, "Input:: Key callback can't access instance's pointer!", ErrorLevel::WARNING);
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
		DEBUG_ASSERT(1, "Input:: Mouse callback can't access instance's pointer!", ErrorLevel::WARNING);
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

void InputListener::setCallback() {
	glfwSetWindowUserPointer(m_window_object, this);
	glfwSetKeyCallback(m_window_object, keyButtonCallback);
	glfwSetMouseButtonCallback(m_window_object, mouseButtonCallback);
}