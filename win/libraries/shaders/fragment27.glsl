//#version 330 core
//out vec4 FragColor;
//
//in VS_OUT {
//	vec3 FragPos;
//	vec3 Normal;
//	vec2 TexCoords;
//	vec4 FragPosLightSpace;
//} fs_in;
//
//uniform sampler2D diffuseTexture;
//uniform sampler2D shadowMap;
//
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//
//float ShadowCalculation(vec4 fragPosLightSpace, float bias)
//{
//		// perform perspective divide
//	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//		// transform to [0,1] range
//	projCoords = projCoords * 0.5 + 0.5;
//		// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//	float closestDepth = texture(shadowMap, projCoords.xy).r; 
//		// get depth of current fragment from light's perspective
//	float currentDepth = projCoords.z;
//
//	float shadow = 0.0;
//	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//	for(int x = -1; x <= 1; ++x)
//	{
//		for(int y = -1; y <= 1; ++y)
//		{
//			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
//			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
//		}    
//	}
//	shadow /= 9.0;
//	if(projCoords.z > 1.0)
//		shadow = 0.0;
//	
//	return shadow;
//}
//
//void main()
//{           
//	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
//	vec3 normal = normalize(fs_in.Normal);
//	vec3 lightColor = vec3(1.0);
//		// ambient
//	vec3 ambient = 0.15 * color;
//		// diffuse
//	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//	float diff = max(dot(lightDir, normal), 0.0);
//	vec3 diffuse = diff * lightColor;
//		// specular
//	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//	float spec = 0.0;
//	vec3 halfwayDir = normalize(lightDir + viewDir);  
//	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//	vec3 specular = spec * lightColor;    
//		// calculate shadow
//	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
//	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);
//	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
//	
//	FragColor = vec4(lighting, 1.0);
//}



#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

float ShadowCalculation(vec3 fragPos)
{
		// get vector between fragment position and light position
	vec3 fragToLight = fragPos - lightPos;
		// ise the fragment to light vector to sample from the depth map    
	float closestDepth = texture(depthMap, fragToLight).r;
		// it is currently in linear range between [0,1], let's re-transform it back to original depth value
	closestDepth *= far_plane;
		// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
		// test for shadows
	float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
	float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
		// display closestDepth as debug (to visualize depth cubemap)
		// FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
	
	return shadow;
}

void main()
{           
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(0.3);
		// ambient
	vec3 ambient = 0.3 * color;
		// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
		// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;    
		// calculate shadow
	float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;                      
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
	
	FragColor = vec4(lighting, 1.0);
}
