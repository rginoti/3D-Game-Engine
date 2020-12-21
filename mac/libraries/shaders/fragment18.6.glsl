#version 330 core

out vec3 FragColor

in vec3 textureDir;

uniform samplerCube cubemap;

int main()
{
	FragColor = texture(cubemap, textureDir);
}
