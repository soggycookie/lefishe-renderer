#vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 VertexColor;
out vec3 normal;
out vec3 tangent;

layout(std140, binding = 0) uniform Camera{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	VertexColor = aColor;
	normal = vec3(model * vec4(aNormal, 1.0));
	tangent = vec3(model * vec4(aTangent, 1.0));
}


#fragment

#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertexColor;
in vec3 normal;
in vec3 tangent;

uniform int here[2] = int[](2, 1);

//layout(binding = 1) uniform sampler2D tex[2];
// layout(binding = 1) uniform sampler2D arr[3];

void main()
{
	// vec4 col = texture(tex[0], TexCoord);
	// vec4 col2 = texture(tex[1], TexCoord);
	FragColor = pow(vec4(TexCoord, here[0] - here[1], 1.0), vec4(1/2.2)) ;
}