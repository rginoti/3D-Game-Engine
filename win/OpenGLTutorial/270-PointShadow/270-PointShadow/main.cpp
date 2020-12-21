#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SHADER/shader.h>
#include <CAMERA/camera.h>
#include <MODEL/model.h>

#include <iostream>

using uint = unsigned int;

	// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool shadows = false;
bool shadowsKeyPressed = false;

	// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPositions[] = {
	glm::vec3(-3.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3 (1.0f, 0.0f, 0.0f),
	glm::vec3 (3.0f, 0.0f, 0.0f)
};
glm::vec3 lightColors[] = {
	glm::vec3(0.25),
	glm::vec3(0.50),
	glm::vec3(0.75),
	glm::vec3(1.00)
};
float cubePositions[] =
{
	-0.5f, 0.5f, 0.5f,		// Front top-left
	-0.5f, -0.5f, 0.5f,		// Front bottom-left
	0.5f, -0.5f, 0.5f,		// Front bottom-right

	-0.5f, 0.5f, 0.5f,		// Front top-left
	0.5f, -0.5f, 0.5f,		// Front bottom-right
	0.5f, 0.5f, 0.5f,		// Front top-right


	0.5f, 0.5f, 0.5f,		// Right-Side top-left
	0.5f, -0.5f, 0.5f,		// Right-Side bottom-left
	0.5f, -0.5f, -0.5f,		// Right-Side bottom-right

	0.5f, 0.5f, 0.5f,		// Right-Side top-left
	0.5f, -0.5f, -0.5f,		// Right-Side bottom-right
	0.5f, 0.5f, -0.5f,		// Right-Side top-right


	-0.5f, 0.5f, -0.5f,		// Left-Side top-left
	-0.5f, -0.5f, -0.5f,	// Left-Side bottom-left
	-0.5f, -0.5f, 0.5f,		// Left-Side bottom-right

	-0.5f, 0.5f, -0.5f,		// Left-Side top-left
	-0.5f, -0.5f, 0.5f,		// Left-Side bottom-right
	-0.5f, 0.5f, 0.5f,		// Left-Side top-right


	0.5f, 0.5f, -0.5f,		// Back top-left
	0.5f, -0.5f, -0.5f,		// Back bottom-left
	-0.5f, -0.5f, -0.5f,	// Back bottom-right

	0.5f, 0.5f, -0.5f,		// Back top-left
	-0.5f, -0.5f, -0.5f,	// Back bottom-right
	-0.5f, 0.5f, -0.5f,		// Back top-right


	0.5f, 0.5f, 0.5f,		// Top top-left
	0.5f, 0.5f, -0.5f,		// Top bottom-left
	-0.5f, 0.5f, -0.5f,		// Top bottom-right

	0.5f, 0.5f, 0.5f,		// Top top-left
	-0.5f, 0.5f, -0.5f,		// Top bottom-right
	-0.5f, 0.5f, 0.5f,		// Top top-right


	-0.5f, -0.5f, 0.5f,		// Bottom top-left
	-0.5f, -0.5f, -0.5f,	// Bottom bottom-left
	0.5f, -0.5f, -0.5f,		// Bottom bottom-right

	-0.5f, -0.5f, 0.5f,		// Bottom top-left
	0.5f, -0.5f, -0.5f,		// Bottom bottom-right
	0.5f, -0.5f, 0.5f,		// Bottom top-right
};
float cubeNormals[] =
{
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	
	1.0f,  0.0f, 0.0f,
	1.0f,  0.0f, 0.0f,
	1.0f,  0.0f, 0.0f,
	1.0f,  0.0f, 0.0f,
	1.0f,  0.0f, 0.0f,
	1.0f,  0.0f, 0.0f,
	
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	
	0.0f, 1.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
};
float cubeTexCoords[] =
{
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
};
glm::vec3 cubeTranslations[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
//float planeVertices[] = {
//		// positions            // normals         // texcoords
//	10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
//	-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
//	-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
//	
//	10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
//	-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
//	10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
//};
float planeVertices[] = {
		// positions            // normals         // texcoords
	25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	
	25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
};

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
	{
		shadows = !shadows;
		shadowsKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		shadowsKeyPressed = false;
	}
}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	
	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		GLenum dataFormat;
		if (nrComponents == 1)
			format = dataFormat = GL_RED;
		else if (nrComponents == 3)
		{
			format = GL_SRGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_SRGB_ALPHA;
			dataFormat = GL_RGBA;
		}
		
		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}

void setPointLightUniform(uint lightCount, glm::vec3 *lightPosition, glm::vec3 *lightColor, Shader *shader)
{
	for (GLuint i = 0; i < lightCount; i++)
	{
		std::string number = std::to_string(i);
		
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].position").c_str()), lightPosition[i].x, lightPosition[i].y, lightPosition[i].z);
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].ambient").c_str()), lightColor[i].x * 0.1f, lightColor[i].y * 0.1f, lightColor[i].z * 0.1f);
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].diffuse").c_str()), lightColor[i].x, lightColor[i].y, lightColor[i].z);
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].quadratic").c_str()), 0.032f);
	}
}

void renderCube();
	// renders the 3D scene
	// --------------------
void renderScene(const Shader &shader)
{
		// room cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(5.0f));
	shader.setMat4("model", model);
	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);
		// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.setMat4("model", model);
	renderCube();
}

	// renderCube() renders a 1x1 3D cube in NDC.
	// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
		// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
				// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
			// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
		// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}





int main()
{
		// glfw: initialize and configure
		// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_SAMPLES, 4);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	
		// glfw window creation
		// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
		// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
		// glad: load all OpenGL function pointers
		// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
		// configure global opengl state
		// -----------------------------
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
	
		// build and compile shaders
		// -------------------------
	Shader shader("../../../libraries/shaders/vertex27.glsl", "../../../libraries/shaders/fragment27.glsl");
	Shader simpleDepthShader("../../../libraries/shaders/vertex27DepthTester.glsl", "../../../libraries/shaders/fragment27DepthTester.glsl", "../../../libraries/shaders/geometry27DepthTester.glsl");
	
//	stbi_set_flip_vertically_on_load(true);
	unsigned int woodTexture = loadTexture("../../../assets/wood.png");
	
		// Increasing these two values makes shadows more accurate.
			// Setting it to 1024 * 10 makes the demo lag on mac.
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("depthMap", 1);

	
		// lighting info
		// -------------
	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	
	
	int frameCount = 0;
	float subtractTime = 0;
	
		// draw as wireframe
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
		// render loop
		// -----------
	while (!glfwWindowShouldClose(window))
	{
			// per-frame time logic
			// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
			// input
			// -----
		processInput(window);
		
			// move light position over time
		lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;
		
			// render
			// ------
			
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
			// 0. create depth cubemap transformation matrices
			// -----------------------------------------------
		float near_plane = 1.0f;
		float far_plane  = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		
			// 1. render scene to depth cubemap
			// --------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		simpleDepthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
		simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		simpleDepthShader.setFloat("far_plane", far_plane);
		simpleDepthShader.setVec3("lightPos", lightPos);
		renderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
			// 2. render scene as normal 
			// -------------------------
		glViewport(0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
			// set lighting uniforms
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.Position);
		shader.setInt("shadows", shadows); // enable/disable shadows by pressing 'SPACE'
		shader.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderScene(shader);
		glm::mat4 model;
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		shader.setMat4("model", model);
		renderCube();
//		std::cout << (shadows ? "true" : "false") << std::endl;
		
		
		frameCount++;
		if(currentFrame - subtractTime >= 1.0)
		{
			std::cout << "FPS: " << frameCount << std::endl;
			currentFrame = 0.0;
			frameCount = 0;
			subtractTime += 1.0;
		}
		
			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
//	glDeleteVertexArrays(1, &cubeVAO);
//	glDeleteBuffers(1, &cubeVBO);
	
	glfwTerminate();
	return 0;
}
