

#include "window.h"
#include "scene.h"
#include "object_loader.h"
#include "render_command_queue.h"
#include "renderer.h"
#include "program_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "material.h"
#include "texture_loader.h"
#include "framebuffer.h"

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
	//glfwWindowHint(GLFW_DEPTH_BITS, 24);
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
	//Renderer renderer;
	RenderCommandQueue queue;
	
	auto program_manager = std::make_shared<ProgramManager>();
	program_manager->add(
		{
			{DEFAULT_PROGRAM, ProgramFactory::createProgram("resource/shader/common.shader")},
			{SCREEN_SPACE_PROGRAM, ProgramFactory::createProgram("resource/shader/screen_space_quad.shader")}
		}
	);

	auto texture_manager = std::make_shared<TextureManager>();
	texture_manager->createTexture(DEFAULT_DIFFUSE_PATH);
	auto material_manager = std::make_shared<MaterialManager>(program_manager, texture_manager);
	std::unique_ptr<ObjectLoader> loader = std::make_unique<AssimpObjectLoader>(material_manager, texture_manager);

	//const auto obj3 = loader->loadObject("resource/asset/dragon/dragon.obj");
	//auto obj2 = loader->loadObject("resource/asset/sphere/scene.gltf");
	//auto obj4 = loader->loadObject("resource/asset/sphere/scene.gltf");
	//auto obj5 = loader->loadObject("resource/asset/sphere/scene.gltf");
	//auto obj6 = loader->loadObject("resource/asset/dragon/dragon.obj");
	auto gun = loader->loadObject("resource/asset/drakefire_pistol/scene.gltf");
	auto obj = loader->loadObject("resource/asset/sphere/scene.gltf");
	//obj4->addChild(obj5);
	//obj5->addChild(obj2);
	//obj2->addChild(obj6);

	auto t = obj->getComponent<TransformComponent>();
	//auto t4 = obj4->getComponent<TransformComponent>();
	//auto t5 = obj5->getComponent<TransformComponent>();
	//auto t2 = obj2->getComponent<TransformComponent>();
	//auto t6 = obj6->getComponent<TransformComponent>();
	auto gun_t = gun->getComponent<TransformComponent>();

	t->position() += VEC3(0, 0, -5);
	//t4->position() += VEC3(0, 0, -5);
	//t5->position() += VEC3(3, 0, 0);
	//t2->position() += VEC3(0, 3, 0);
	//t6->position() += VEC3(0, 0, 3);
	//gun_t->rotation() += VEC3(0, 90, 90);
	//gun_t->position() += VEC3(0, 0, -1);
	//gun_t->scale() = VEC3(0.5, 0.5, 0.5);
	


	auto cam  = ObjectFactory::create();
	cam->addComponent<CameraComponent>();

	auto camera = cam->getComponent<CameraComponent>();
	CameraComponent::main(camera);
	auto transform = cam->getComponent<TransformComponent>();
	transform->position() += VEC3(0, 0, 3);

	scene.addObject(std::move(cam));
	scene.addObject(std::move(obj));
	//scene.addObject(std::move(obj4));
	scene.addObject(std::move(gun));

	auto ssmat = std::make_shared<Material>(program_manager->getProgram(SCREEN_SPACE_PROGRAM));

	
	
	ScreenSpaceQuad m_quad;

	while (!glfwWindowShouldClose(m_window_obj))
	{
		setCurrentTime();
		setDeltaTime();





		//t4->rotation() += VEC3(0, 45, 0) * VEC3(0, m_framedata.delta_time, 0);
		//t5->rotation() += VEC3(45, 0, 0) * VEC3(m_framedata.delta_time, 0, 0);
		//t6->rotation() += VEC3(0, 0, 45) * VEC3(0, 0, m_framedata.delta_time);
		gun_t->rotation() += VEC3(0, 30 * m_framedata.delta_time, 0);

		scene.update();

		queue.submitAndDraw(scene.cameraObjects(), scene.renderObjects());

		Framebuffer::unbind();
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);   
		ssmat->assignTexture("mainTex", CameraComponent::main()->colorTexture());
		ssmat->bindAndSetUniform();

		m_quad.bind();

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