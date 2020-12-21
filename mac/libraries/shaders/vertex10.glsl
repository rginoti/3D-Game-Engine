#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

	FragPos = vec3(model * vec4(aPos, 1.0));

		// Inversing matrices is a costly operation. In a real application do the calculation on the cpu and send the info to the shader.
	Normal = mat3(transpose(inverse(model))) * aNormal;

	TexCoords = aTexCoord;

		// Note we read the multiplication from right to left.
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
