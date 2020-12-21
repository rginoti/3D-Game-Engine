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
bool blinn = false;
bool blinnKeyPressed = false;
bool gammaEnabled = false;
bool gammaKeyPressed = false;

	// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int planeVAO, planeVBO;

glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f,  0.4f,  0.0f),
	glm::vec3(0.0f,  -2.0f,  -3.4f),
	glm::vec3(0.0f,  5.0f,  -10.0f),
	glm::vec3(-2.0f,  3.0f,  -20.0f),
};

glm::vec3 pointLightColors1[] = {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0, 1.0),
	glm::vec3(1.0f, 1.0f, 1.0f)
};
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
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed) 
	{
		blinn = !blinn;
		blinnKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) 
	{
		blinnKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
	{
		gammaEnabled = !gammaEnabled;
		gammaKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		gammaKeyPressed = false;
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
unsigned int loadTexture(char const *path, bool gammaCorrection)
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
//			format = gammaCorrection ? GL_SRGB : GL_RGB;
			format = GL_SRGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
//			format = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
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
//		// floor
//	glm::mat4 model = glm::mat4(1.0f);
//	shader.setMat4("model", model);
//	glBindVertexArray(planeVAO);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//		// cubes
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(-5.0f, 1.5f, -5.0));
//	model = glm::scale(model, glm::vec3(0.5f));
//	shader.setMat4("model", model);
//	renderCube();
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -4.0));
//	model = glm::scale(model, glm::vec3(0.5f));
//	shader.setMat4("model", model);
//	renderCube();
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -3.0));
//	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
//	model = glm::scale(model, glm::vec3(0.25));
//	shader.setMat4("model", model);
//	renderCube();
	
		// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
		// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
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

	// renderQuad() renders a 1x1 XY quad in NDC
	// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
				// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
			// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
	Shader shader("../../../libraries/shaders/vertex26.glsl", "../../../libraries/shaders/fragment26.glsl");
	Shader debugDepthQuad("../../../libraries/shaders/vertex26DepthQuad.glsl", "../../../libraries/shaders/fragment26DepthQuad.glsl");
	Shader simpleDepthShader("../../../libraries/shaders/vertex26DepthTester.glsl", "../../../libraries/shaders/fragment26DepthTester.glsl");
	
		// plane VAO
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	
//	stbi_set_flip_vertically_on_load(true);
	unsigned int woodTexture = loadTexture("../../../assets/wood.png", true);
	
		// Increasing these two values makes shadows more accurate.
			// Setting it to 1024 * 10 makes the demo lag on mac.
	const unsigned int SHADOW_WIDTH = 1024 * 5, SHADOW_HEIGHT = 1024 * 5;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
//			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	unsigned int depthMap;
		// Generates an ID for the texture.
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
		// glTexImage2D generates a texture for the texture binded to the target (texture binded to GL_TEXTURE_2D).
		// This call creates the dimensions for the texture. The texture was just an empty object beforehand.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		// This call binds a texture object to the framebuffer target.
		// It tells the gpu that drawing calls on this framebuffer will store depth values in this texture (depthMap).
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
	
		// lighting info
		// -------------
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
//	glm::vec3 lightPos(-7.0f, 4.0f, -6.0f);
	
	
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
		
		
			// render
			// ------
			
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
//			// 1. render depth of scene to texture (from light's perspective)
//			// --------------------------------------------------------------
//		glm::mat4 lightProjection, lightView;
//		glm::mat4 lightSpaceMatrix;
//		float near_plane = 1.0f, far_plane = 7.5f;
//		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
//		lightSpaceMatrix = lightProjection * lightView;
//			// render scene from light's point of view
//		simpleDepthShader.use();
//		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
//		
//		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//		glClear(GL_DEPTH_BUFFER_BIT);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, woodTexture);
//		renderScene(simpleDepthShader);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		
//			// reset viewport
//		glViewport(0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		
//			// render Depth map to quad for visual debugging
//			// ---------------------------------------------
//		debugDepthQuad.use();
//		debugDepthQuad.setFloat("near_plane", near_plane);
//		debugDepthQuad.setFloat("far_plane", far_plane);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, depthMap);
//		renderQuad();

		
			// 1. render depth of scene to texture (from light's perspective)
			// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//		lightProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
			// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, woodTexture);
			// Culling the front faces solves peter panning by hiding the disconnected part of the shadow inside the geometry.
//		glCullFace(GL_FRONT);
		renderScene(simpleDepthShader);
//		glCullFace(GL_BACK); // don't forget to reset original culling face
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
			// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			// 2. render scene as normal using the generated depth/shadow map
			// --------------------------------------------------------------
		glViewport(0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
			// set light uniforms
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(shader);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(lightPos.x + -10, lightPos.y + -3, lightPos.z));
		model = glm::scale(model, glm::vec3(0.2f));
		shader.setMat4("model", model);
		renderCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		shader.setMat4("model", model);
		renderCube();
		
		
			// render Depth map to quad for visual debugging
			// ---------------------------------------------
		debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
//			renderQuad();
		
		
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
