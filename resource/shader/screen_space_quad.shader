#vertex
#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 0.0, 1.0);
	TexCoord = aTexCoord;
}


#fragment

#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

layout(binding = 0) uniform sampler2D mainTex;


void main()
{
	FragColor = pow(texture(mainTex, TexCoord), vec4(1/2.2)) ;
}