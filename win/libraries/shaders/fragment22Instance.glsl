//#version 330 core
//out vec4 FragColor;
//
//in vec3 fColor;
//
//void main()
//{
//	FragColor = vec4(fColor, 1.0);
//}


//#version 330 core
//
//struct Material {
//	sampler2D texture_diffuse1;
//	sampler2D texture_specular1;
//	float shininess;
//}; 
//
//struct DirLight {
//	vec3 direction;
//	
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//};
//
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
//
//struct SpotLight {
//	vec3 position;
//	vec3 direction;
//	float innerCutOff;
//	float outerCutOff;
//	
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//	
//	float constant;
//	float linear;
//	float quadratic;
//};
//
//
//in VS_OUT
//{
//	vec3 Normal;
//	vec3 FragPos;
//	vec2 TexCoords;
//} fs_in;
//
//uniform vec3 viewPos;
//uniform Material material;
//uniform DirLight dirLight;
//#define NR_POINT_LIGHTS 4  
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform SpotLight spotLight;
//uniform vec3 texture_normal1;
//
//out vec4 FragColor;
//
//
//	// calculates the color when using a directional light.
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//	vec3 lightDir = normalize(-light.direction);
//		// Diffuse Shading
//	float diff = max(dot(normal, lightDir), 0.0);
//		// Specular Shading
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//		// Combine Results
//	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
//	return (ambient + diffuse + specular);
//}
//
//	// calculates the color when using a point light.
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
//	vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
//	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
//	ambient  *= attenuation;
//	diffuse  *= attenuation;
//	specular *= attenuation;
//	return (ambient + diffuse + specular);
//} 
//
//	// calculates the color when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//	vec3 lightDir = normalize(light.position - fragPos);
//		// diffuse shading
//	float diff = max(dot(normal, lightDir), 0.0);
//		// specular shading
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//		// attenuation
//	float distance = length(light.position - fragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//		// spotlight intensity
//	float theta = dot(lightDir, normalize(-light.direction)); 
//	float epsilon = light.innerCutOff - light.outerCutOff;
//	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//		// combine results
//	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
//	ambient *= attenuation * intensity;
//	diffuse *= attenuation * intensity;
//	specular *= attenuation * intensity;
//	return (ambient + diffuse + specular);
//}
//
//void main()
//{
//		// properties
//	vec3 norm = normalize(fs_in.Normal);
//	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//	
//		// == =====================================================
//		// Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
//		// For each phase, a calculate function is defined that calculates the corresponding color
//		// per lamp. In the main() function we take all the calculated colors and sum them up for
//		// this fragment's final color.
//		// == =====================================================
//		// phase 1: directional lighting
//	vec3 result = vec3(0.0);
//		//	vec3 result = CalcDirLight(dirLight, norm, viewDir);
//		// phase 2: point lights
//	for(int i = 0; i < NR_POINT_LIGHTS; i++)
//		result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);    
//		// phase 3: spot light
//		//	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
//	
//		//	FragColor = vec4(result, 1.0);
//	FragColor = vec4(result, 1.0);
//}


#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords);
}
