#fragment

#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertexColor;
in vec3 normal;
in vec3 tangent;



void main()
{
	FragColor = vec4(1.0);
}