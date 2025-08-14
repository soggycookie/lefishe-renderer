#include "renderer.h"
#include "shader.h"
#include "program.h"

//#include "texture_loader.h"
//#include "texture.h"

using namespace Lefishe;

void Renderer::draw(const std::vector<DrawCall>& drawcalls){	


	for(const auto& drawcall : drawcalls){
		drawcall.bind();


		glDrawElements(GL_TRIANGLES, drawcall.indexBuffer().size(), GL_UNSIGNED_INT, (void*)0);
	}
}

