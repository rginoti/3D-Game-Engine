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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool blinn = false;
bool blinnKeyPressed = false;

	// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			format = 0;
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
	glDepthFunc(GL_LESS);
//	glEnable(GL_MULTISAMPLE);
	
		// build and compile shaders
		// -------------------------
	Shader shader("../../../libraries/shaders/vertex24.glsl", "../../../libraries/shaders/fragment24.glsl");
	Shader lampShader("../../../libraries/shaders/vertex8Lamp.txt", "../../../libraries/shaders/fragment8Lamp.txt");
	
		// Cube VAO.
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindVertexArray(cubeVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions) + sizeof(cubeNormals) + sizeof(cubeTexCoords), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubePositions), &cubePositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubePositions), sizeof(cubeNormals), &cubeNormals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubePositions) + sizeof(cubeNormals), sizeof(cubeTexCoords), &cubeTexCoords);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(cubePositions)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(cubePositions) + sizeof(cubeNormals)));
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	unsigned int matricesIndex = glGetUniformBlockIndex(shader.ID, "Matrices");
	glUniformBlockBinding(shader.ID, matricesIndex, 0);
	unsigned int matricesUBO;
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), 0, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));
	
	stbi_set_flip_vertically_on_load(true);
	unsigned int diffuseMap = loadTexture("../../../assets/chest.png");
	unsigned int specularMap = loadTexture("../../../assets/chest_specular.png");
	
	shader.use();
	shader.setInt("material.texture_diffuse1", 0);
	shader.setInt("material.texture_specular1", 1);
	
	
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
			
			// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection;
		glm::mat4 view;
		
		shader.use();
		shader.setFloat("material.shininess", 32.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		
		shader.setVec3("viewPos", camera.Position);
		
		glBindVertexArray(cubeVAO);
		view = camera.GetViewMatrix();
//		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
//		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
//		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		setPointLightUniform(4, pointLightPositions, pointLightColors1, &shader);
//		for(int i = 0; i < (sizeof(cubeTranslations) / (3 * sizeof(float))); i++)
		shader.setBool("blinn", blinn);
		std::cout << blinn << std::endl;
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubeTranslations[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		lampShader.use();
		glBindVertexArray(lightVAO);
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		for(int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(.2));
			lampShader.setMat4("model", model);
			lampShader.setVec3("Color", pointLightColors1[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		
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
