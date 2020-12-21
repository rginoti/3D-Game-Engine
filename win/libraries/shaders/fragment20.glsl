
#version 330 core
	// Need to change glsl version so depth condition can work.
		// The macOS at this time only supports 4.1
//#version 420 core

	// This is the gl_FragDepth depth condition.
		// The condition can be: any/greater/less/unchanged.
			// The last 3 keywords will trigger the early depth testing to work with some knowledge on what the fragment depth will be.
//layout (depth_<condition>) out float gl_FragDepth;
//layout (depth_greater) out float gl_FragDepth;

out vec4 FragColor;

//in vec2 TexCoords;
//in VS_OUT
//{
//	vec2 TexCoords;
//} fs_in;

//uniform sampler2D frontTexture;
//uniform sampler2D backTexture;

void main()
{
		// We can change the depth of the fragment using gl_FragDepth.
			// WARNING: Cannot use early depth testing with gl_FragDepth unless we redeclare gl_FragDepth with a depth condition.
				// Will not work on the present macOS.
//	gl_FragDepth = gl_FragCoord.z + .1;
//	gl_FragDepth = 0.0;
	FragColor = vec4(.6, .6, .6, 1.0);
	
//		// gl_FrontFacing is a bool.
//	if(gl_FrontFacing)
//	{
//		FragColor = vec4(0.0, .5, .5, 1.0);
//	}
//	else
//	{
//		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//	}
	
		// Color of fragment depends on location on the screen.
//	FragColor = vec4(gl_FragCoord.x / 2000, .5, gl_FragCoord.y / 2000, 1.0);
	
//	if(gl_FragCoord.x < 400)
//	{
//		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//	}
//	else
//	{
//		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//	}
	
//	FragColor = vec4(0.3, 0.0, 0.0, 0.0);
}
