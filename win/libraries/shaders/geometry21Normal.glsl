//#version 330 core
//
//	// The input layout qualifier can be points/lines/lines_adjacency/triangles/triangles_adjacency.
//layout (points) in;
//	// The output layout qualifier can be points/line_strip/triangle_strip.
//layout (line_strip, max_vertices = 100) out;
//
//void main() 
//{
//	for(float i = 0.0; i < 1.0; i += .0005)
//	{
//	vec4 temp1 = gl_in[0].gl_Position + vec4(-0.1, i, 0.0, 0.0); 
//	gl_Position = temp1;
//	EmitVertex();
//	
//	vec4 temp2 = gl_in[0].gl_Position + vec4( 0.1, i, 0.0, 0.0);
//	gl_Position = temp2;
//	EmitVertex();
//	
//	EndPrimitive();
//	}
//	
////	for(float i = 0.0; i < 1.0; i += .1)
////	{
////		gl_Position = 
////	}
//}


//#version 330 core
//layout (points) in;
//layout (points, max_vertices = 1) out;
//
//void main() {    
//	gl_Position = gl_in[0].gl_Position; 
//	EmitVertex();
//	EndPrimitive();
//}


//#version 330 core
//
//layout (points) in;
//// A triangle_strip vertex will always use the last two vertices for its triangle.
//layout (triangle_strip, max_vertices = 5) out;
//
//in VS_OUT
//{
//	vec3 color;
//} gs_in[];
//
//out vec3 fColor;
//
//vec3 GetNormal()
//{
//	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
//	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
//	return normalize(cross(a, b));
//}
//
//vec4 explode(vec4 position, vec3 normal)
//{
//	float magnitude = 2.0;
//	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
//	return position + vec4(direction, 0.0);
//}
//
//void main()
//{
//	fColor = gs_in[0].color;
//	gl_Position = gl_in[0].gl_Position + vec4(-.1, -.1, 0.0, 0.0);
//	EmitVertex();
//	
//	gl_Position = gl_in[0].gl_Position + vec4(.1, -.1, 0.0, 0.0);
//	EmitVertex();
//	
//	gl_Position = gl_in[0].gl_Position + vec4(-.1, .1, 0.0, 0.0);
//	EmitVertex();
//	
//	gl_Position = gl_in[0].gl_Position + vec4(.1, .1, 0.0, 0.0);
//	EmitVertex();
//	
//	gl_Position = gl_in[0].gl_Position + vec4(0.0, .2, 0.0, 0.0);
//	fColor = vec3(1.0, 1.0, 1.0);
//	EmitVertex();
//	
//	EndPrimitive();
//}


//#version 330 core
//
//layout (triangles) in;
//	// A triangle_strip vertex will always use the last two vertices for its triangle.
//layout (triangle_strip, max_vertices = 3) out;
//
//in VS_OUT
//{
//	vec3 normal;
//	vec3 fragPos;
//	vec2 texCoords;
//} gs_in[];
//
//out vec3 Normal;
//out vec3 FragPos;
//out vec2 TexCoords;
//
//uniform float time;
//
//vec3 GetNormal()
//{
//	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
//	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
//	return normalize(cross(a, b));
//}
//
//vec4 explode(vec4 position, vec3 normal)
//{
//	float magnitude = 2.0;
//	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
//	return position + vec4(direction, 0.0);
//}
//
//void main()
//{
//	vec3 normal = GetNormal();
//	
//	for(int i = 0; i < 3; i++)
//	{
//		gl_Position = explode(gl_in[i].gl_Position, normal);
//		TexCoords = gs_in[i].texCoords;
//		Normal = gs_in[i].normal;
//		FragPos = gs_in[i].fragPos;
//		EmitVertex();
//	}
//	
////	for(int i = 0; i < 3; i++)
////	{
////		gl_Position = gl_in[i].gl_Position + vec4(-.1, -.1, 0.0, 0.0);
////		TexCoords = gs_in[i].texCoords;
////		Normal = gs_in[i].normal;
////		FragPos = gs_in[i].fragPos;
////		EmitVertex();
////	}
//	EndPrimitive();
//}


#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;

void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0); // first vertex normal
	GenerateLine(1); // second vertex normal
	GenerateLine(2); // third vertex normal
}
