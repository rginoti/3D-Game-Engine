//#version 330 core
//
//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//uniform sampler2D texture1;
//
//void main()
//{
//	FragColor = texture(texture1, TexCoords);
//}

#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
//in vec2 TexCoords

uniform vec3 cameraPos;
uniform samplerCube skybox;

//	 Reflection
void main()
{             
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
	
	// Refraction
//void main()
//{             
//	float ratio = 1.00 / 1.52;
//	vec3 I = normalize(Position - cameraPos);
//	vec3 R = refract(I, normalize(Normal), ratio);
//	FragColor = vec4(texture(skybox, R).rgb, 1.0);
//}

//#version 330 core


//struct Material {
//	sampler2D texture_diffuse1;
//	sampler2D texture_ambient1;
//	sampler2D texture_specular1;
//	float shininess;
//}; 

//struct PointLight {
//	vec3 position;
//	
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//	
//	float constant;
//	float linear;
//	float quadratic;
//};


//in vec3 Normal;
//in vec3 Position;
//in vec3 FragPos;
//in vec2 TexCoords;

//uniform vec3 viewPos;
//uniform Material material;
//uniform DirLight dirLight;
//#define NR_POINT_LIGHTS 4  
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform SpotLight spotLight;
//uniform sampler2D texture_ambient1;
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform vec3 texture_normal1;
//uniform vec3 cameraPos;
//uniform samplerCube skybox;
//
//out vec4 FragColor;


	// calculates the color when using a point light.
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//	vec3 lightDir = normalize(light.position - fragPos);
//		// diffuse shading
//	float diff = max(dot(normal, lightDir), 0.0);
//		// specular shading
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//		// attenuation
//	float distance    = length(light.position - fragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * distance + 
//							   light.quadratic * (distance * distance));    
//		// combine results
//	vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
//	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
//	ambient  *= attenuation;
//	diffuse  *= attenuation;
//	specular *= attenuation;
//	return (ambient + diffuse + specular);
//} 

//vec3 CalcReflection()
//{
//	
//	vec3 result = vec3(texture(material.texture_diffuse1, TexCoords));
//	return result;
//}
//
//
//void main()
//{
		// properties
//	vec3 norm = normalize(Normal);
//	vec3 viewDir = normalize(viewPos - FragPos);
	
		// == =====================================================
		// Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
		// For each phase, a calculate function is defined that calculates the corresponding color
		// per lamp. In the main() function we take all the calculated colors and sum them up for
		// this fragment's final color.
		// == =====================================================
		// phase 1: directional lighting
//	vec3 result = vec3(0.0);
//	vec3 result = CalcDirLight(dirLight, norm, viewDir);
		// phase 2: point lights
//	result += CalcPointLight(pointLights, norm, FragPos, viewDir);
//	vec3 I = normalize(Position - cameraPos);
//	vec3 R = reflect(I, normalize(Normal));
//	vec3 skyboxLocation = vec3(texture(skybox, R).rgb);
//	vec3 reflection = skyboxLocation * vec3(texture(material.texture_ambient1, TexCoords));
//	result += CalcReflection();
//	result += reflection;
		// phase 3: spot light
//	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	
//	FragColor = vec4(result, 1.0);
//	FragColor = vec4(1.0);
//}

