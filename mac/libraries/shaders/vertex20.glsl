#version 330 core

layout (location = 0) in vec3 aPos;

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

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);  
		// Change the size of the points if rendering GL_POINTS.
//	gl_PointSize = gl_Position.z;
	
		// Using an interface block to output TexCoords.
//	vs_out.TexCoords = aTexCoords;
}
