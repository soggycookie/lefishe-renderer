

#include "window.h"
#include "scene.h"
#include "object_loader.h"
#include "render_command_queue.h"
#include "renderer.h"
#include "program_manager.h"
#include "material_manager.h"
#include "material.h"
#include "texture_loader.h"

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
		LOG_ERROR("GLFW:: Failed to init!");
		
		return false;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window_obj = glfwCreateWindow(m_config.screen_width, m_config.screen_height, m_config.name.data(), NULL, NULL);
	if (!m_window_obj)
	{
		LOG_ERROR("GLFW:: Failed to create Window!");
		
		terminate();
		return false;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(m_window_obj);
	glfwSwapInterval(1);
	return true;
}

bool Window::initOpenGLLoader() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// Handle initialization failure
		LOG_ERROR("GLAD:: Failed to load OpenGL!");
		
		return false;
	}

	return true;
}

void Window::config() {

}

void Window::update() {
	Scene scene;
	Renderer renderer;
	RenderCommandQueue queue;
	
	auto program_manager = std::make_shared<ProgramManager>();
	program_manager->add(
		{
			{DEFAULT_PROGRAM, ProgramFactory::createProgram("resource/shader/common.shader")}
		}
	);

	auto material_manager = std::make_shared<MaterialManager>(program_manager);

	std::unique_ptr<ObjectLoader> loader = std::make_unique<AssimpObjectLoader>(material_manager);

	//auto obj = loader->loadObject("resource/asset/sphere/scene.gltf");
	//const auto obj3 = loader->loadObject("resource/asset/dragon/dragon.obj");
	auto obj2 = loader->loadObject("resource/asset/sphere/scene.gltf");
	auto obj4 = loader->loadObject("resource/asset/sphere/scene.gltf");
	auto obj5 = loader->loadObject("resource/asset/sphere/scene.gltf");
	obj4->addChild(obj5);
	obj5->addChild(obj2);

	//auto t = obj->getComponent<TransformComponent>();
	auto t4 = obj4->getComponent<TransformComponent>();
	auto t5 = obj5->getComponent<TransformComponent>();
	auto t2 = obj2->getComponent<TransformComponent>();

	//t->position() += VEC3(-4, 0, -5);

	t4->position() += VEC3(0, 0, -5);
	t5->position() += VEC3(3, 0, 0);
	t2->position() += VEC3(0, 3, 0);

	

	auto cam  = ObjectFactory::create();
	cam->addComponent<CameraComponent>();

	auto camera = cam->getComponent<CameraComponent>();
	CameraComponent::main(camera);
	auto transform = cam->getComponent<TransformComponent>();
	transform->position() += VEC3(0, 0, 3);

	//auto texture = TextureLoader::load("resource/asset/texture/container.jpg");


	scene.addObject(std::move(cam));
	scene.addObject(std::move(obj4));


	queue.submit(scene.renderObjects());
	
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	while (!glfwWindowShouldClose(m_window_obj))
	{
		setCurrentTime();
		setDeltaTime();

		glClear(GL_COLOR_BUFFER_BIT);

		t4->rotation() += VEC3(0, 45, 0) * VEC3(0, m_framedata.delta_time, 0);
		t5->rotation() += VEC3(45, 0, 0) * VEC3(m_framedata.delta_time, 0, 0);
		

		scene.update();
		
		renderer.draw(queue.drawcall());

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