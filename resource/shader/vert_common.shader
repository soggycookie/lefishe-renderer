#vertex
#version core 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 VertexColor;
out vec3 normal;
out vec3 tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	VertexColor = aColor;
	normal = vec3(model * vec4(aNormal, 1.0));
	tangent = vec3(model * vec4(aTangent, 1.0));
}