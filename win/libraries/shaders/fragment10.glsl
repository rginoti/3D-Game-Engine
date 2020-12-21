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
		// No longer necessary when using directional lights.
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
    	// diffuse 
    vec3 norm = normalize(Normal);
//	vec3 directionLightDir = normalize(-light.direction);
//	vec3 pointLightDir = normalize(pointLight.position - FragPos);
	vec3 spotLightDir = normalize(spotLight.position - FragPos);
//	float directionalLightDiff = max(dot(norm, directionalLightDir), 0.0);
//	float pointLightDiff = max(dot(norm, pointLightDir), 0.0);
	
    	// Specular Calculations
    vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 pointLightReflectDir = reflect(-pointLightDir, norm);
//	float directionalLightSpec = pow(max(dot(viewDir, directionalLightReflectDir), 0.0), material.shininess);
//	float pointLightSpec = pow(max(dot(viewDir, pointLightReflectDir), 0.0), material.shininess);
	
		// Attenuation
	float attenuation = 0;
//	float pointLightDistance = length(pointLight.position - FragPos);
//	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	
//	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, TexCoords));
//	vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//	vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, TexCoords));
	
	vec3 spotLightAmbient;
	vec3 spotLightDiffuse;
	vec3 spotLightSpecular;
	
		// Spotlight calculations
	float theta = dot(spotLightDir, normalize(-spotLight.direction));
	if(theta > spotLight.innerCutOff)
	{
		float spotLightDiff = max(dot(norm, spotLightDir), 0.0);
		float spotLightDistance = length(spotLight.position - FragPos);
		vec3 spotLightReflectDir = reflect(-spotLightDir, norm);
		float spotLightSpec = pow(max(dot(viewDir, spotLightReflectDir), 0.0), material.shininess);
		
		spotLightAmbient = spotLight.ambient * vec3(texture(material.diffuse, TexCoords));
		spotLightDiffuse = spotLight.diffuse * spotLightDiff * vec3(texture(material.diffuse, TexCoords));
		spotLightSpecular = spotLight.specular * spotLightSpec * vec3(texture(material.specular, TexCoords));
		
		attenuation = 1.0 / (spotLight.constant + spotLight.linear * spotLightDistance + spotLight.quadratic * (spotLightDistance * spotLightDistance));
		
		spotLightDiffuse *= attenuation;
		spotLightSpecular *= attenuation;
		
		vec3 result = spotLightAmbient + spotLightDiffuse + spotLightSpecular;
		FragColor = vec4(result, 0.1);
	}
	else
	{
		FragColor = vec4(spotLight.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
	}
}
