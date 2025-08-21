#vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 VertexColor;
out mat3 TBN;

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



	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	
	//Gram-Schmidt
	T = normalize(T - dot(T, N) * N);

	vec3 bitangent = cross(N, T);

	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));

	// if (dot(cross(N, T), vec3(model * vec4(cross(aNormal, aTangent), 0.0))) < 0.0) {
	// 	B = -B;
	// }

	TBN = mat3(T, B, N);
}


#fragment

#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertexColor;
in mat3 TBN;



layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D normalMap;
// layout(binding = 2) uniform sampler2D metallicMap;


vec4 color = vec4(1.0);

void main()
{
	vec4 baseColor = texture(diffuseMap, TexCoord);
	vec3 normal = texture(normalMap, TexCoord).xyz;
	normal = normal * 2 - 1;

	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(vec3(1, 1, 0));
	

	float diffuse = clamp(dot(lightDir, normal), 0 , 1);

	vec3 finalColor = baseColor.xyz * diffuse + baseColor.xyz * 0.1;

	//vec3 check = TBN[0] * 0.5 + vec3(0.5);

	FragColor =  vec4(finalColor, 1);
}