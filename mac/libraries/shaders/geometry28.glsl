#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT 
{
	vec2 TexCoords;
	vec3 LightPos;
	vec3 ViewPos;
	vec3 FragPos;
//	vec3 TangentFragPos;
	mat3 TBN;
} gs_in[];

out GS_OUT 
{
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} gs_out;



out vec4 FragPos;

void main()
{
	gs_out.TangentViewPos = gs_in[1].TBN * gs_in[1].ViewPos;
	gs_out.TangentLightPos = gs_in[1].TBN * gs_in[1].LightPos;
	
	
	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		gs_out.TexCoords = gs_in[i].TexCoords;
		gs_out.TangentFragPos = gs_in[1].TBN * gs_in[i].FragPos;
//		gs_out.TangentFragPos = gs_in[i].TangentFragPos;
		EmitVertex();
	}
	EndPrimitive();
}
