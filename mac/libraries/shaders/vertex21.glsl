#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aColor;
//layout (location = 1) in vec2 aTexCoords;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

	// This is an interface block.
		// It looks just like a struct.
//out VS_OUT
//{
//	vec2 TexCoords;
//} vs_out;
//out VS_OUT
//{
//	vec3 color;
//} vs_out;

out VS_OUT
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoords;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
//	vs_out.color = aColor;
	vs_out.texCoords = aTexCoords;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
		// Change the size of the points if rendering GL_POINTS.
//	gl_PointSize = gl_Position.z;
	
		// Using an interface block to output TexCoords.
//	vs_out.TexCoords = aTexCoords;
}
