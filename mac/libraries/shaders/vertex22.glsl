//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec3 aColor;
//
//out vec3 fColor;
//
//uniform vec2 offsets[100];
//
//void main()
//{
//	vec2 offset = offsets[gl_InstanceID];
//	gl_Position = vec4(aPos + offset, 0.0, 1.0);
//	fColor = aColor;
//}


//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aOffset;
//
//out vec3 fColor;
//
////void main()
////{
////	gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
////	fColor = aColor;
////}
//
//void main()
//{
//	vec2 pos = aPos * (gl_InstanceID / 100.0);
//	gl_Position = vec4(pos + aOffset, 0.0, 1.0);
//	fColor = aColor;
//}


#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

//out VS_OUT
//{
//	vec3 Normal;
//	vec3 FragPos;
//	vec2 TexCoords;
//} vs_out;
out vec2 TexCoords;

void main()
{
//	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
//	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
//	vs_out.TexCoords = aTexCoords;
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}


//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 2) in vec2 aTexCoords;
//layout (location = 3) in mat4 instanceMatrix;
//
//layout (std140) uniform Matrices
//{
//	mat4 view;
//	mat4 projection;
//};
//
//out vec2 TexCoords;
//
//void main()
//{
//	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0); 
//	TexCoords = aTexCoords;
//}
