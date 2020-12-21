#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct DirectionalLight {
	vec3 direction;
  
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
 };

struct PointLight {
	vec3 position;
  
    vec3 ambient;
	vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

out vec4 FragColor;

void main()
{
    	// Diffuse Calculations
    vec3 norm = normalize(Normal);
//	vec3 directionalLightDir = normalize(-light.direction);
	vec3 pointLightDir = normalize(pointLight.position - FragPos);
	vec3 spotLightDir = normalize(spotLight.position - FragPos);
//	float directionalLightDiff = max(dot(norm, directionalLightDir), 0.0);
	float pointLightDiff = max(dot(norm, pointLightDir), 0.0);
	float spotLightDiff = max(dot(norm, spotLightDir), 0.0);
	
	
    	// Specular Calculations
    vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 directionalLightReflectDir = reflect(-directionalLightDir, norm);
	vec3 pointLightReflectDir = reflect(-pointLightDir, norm);
	vec3 spotLightReflectDir = reflect(-spotLightDir, norm);
//	float directionalLightSpec = pow(max(dot(viewDir, directionalLightReflectDir), 0.0), material.shininess);
	float pointLightSpec = pow(max(dot(viewDir, pointLightReflectDir), 0.0), material.shininess);
	float spotLightSpec = pow(max(dot(viewDir, spotLightReflectDir), 0.0), material.shininess);
	
	
		// Attenuation Calculations
	float pointLightDistance = length(pointLight.position - FragPos);
	float spotLightDistance = length(spotLight.position - FragPos);
//	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * pointLightDistance + pointLight.quadratic * (pointLightDistance * pointLightDistance));
	float attenuation = (1.0 / (pointLight.constant + pointLight.linear * pointLightDistance + pointLight.quadratic * (pointLightDistance * pointLightDistance))) / 2.0;
//	attenuation += 1.0 / (spotLight.constant + spotLight.linear * spotLightDistance + spotLight.quadratic * (spotLightDistance * spotLightDistance));
	attenuation += (1.0 / (spotLight.constant + spotLight.linear * spotLightDistance + spotLight.quadratic * (spotLightDistance * spotLightDistance))) / 2.0;
	
	
		// Combining Directional/Point/Spot lights
			// Ambient/Diffuse/Specular results
	vec3 pointLightAmbient = pointLight.ambient * vec3(texture(material.diffuse, TexCoords).rgb);
	vec3 pointLightDiffuse = pointLight.diffuse * pointLightDiff * vec3(texture(material.diffuse, TexCoords).rgb);
	vec3 pointLightSpecular = pointLight.specular * pointLightSpec * vec3(texture(material.specular, TexCoords).rgb);
	
	vec3 spotLightAmbient = spotLight.ambient * vec3(texture(material.diffuse, TexCoords).rgb);
	vec3 spotLightDiffuse = spotLight.diffuse * spotLightDiff * vec3(texture(material.diffuse, TexCoords).rgb);
	vec3 spotLightSpecular = spotLight.specular * spotLightSpec * vec3(texture(material.specular, TexCoords).rgb);
	
//	vec3 ambient = (pointLightAmbient / 2.0) + (spotLightAmbient / 2.0);
//	vec3 diffuse = (pointLightDiffuse / 2.0) + (spotLightDiffuse / 2.0);
//	vec3 specular = (pointLightSpecular / 2.0) + (spotLightSpecular / 2.0);
	vec3 ambient = (pointLightAmbient) + (spotLightAmbient);
	vec3 diffuse = (pointLightDiffuse) + (spotLightDiffuse);
	vec3 specular = (pointLightSpecular) + (spotLightSpecular);
	
	
		// Spotlight calculations
	float theta = dot(spotLightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
	
	diffuse *= intensity;
	specular *= intensity;
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
