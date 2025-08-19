#include "renderer.h"
#include "shader.h"
#include "program.h"
#include "framebuffer.h"

//#include "texture_loader.h"
//#include "texture.h"

using namespace Lefishe;

Renderer::Renderer() {

}

void Renderer::draw(DrawCall& drawcall){	
	drawcall.bind();
	glDrawElements(GL_TRIANGLES, drawcall.indexBuffer().size(), GL_UNSIGNED_INT, (void*)0);
	drawcall.cleanUpBinding();
}

