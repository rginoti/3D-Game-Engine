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

	// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

	// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	
	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
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
	Shader shader("/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/vertex23.glsl", "/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/fragment23.glsl");
	Shader screenShader("/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/vertex23_PostAA.glsl", "/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/fragment23_PostAA.glsl");
	
		// Cube VAO.
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindVertexArray(cubeVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubePositions), &cubePositions);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	unsigned int matricesIndex = glGetUniformBlockIndex(shader.ID, "Matrices");
	glUniformBlockBinding(shader.ID, matricesIndex, 0);
	unsigned int matricesUBO;
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), 0, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));
	
		// setup screen VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	
		// configure MSAA framebuffer
		// --------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a multisampled color attachment texture
	unsigned int textureColorBufferMultiSampled;
	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH * 2, SCR_HEIGHT * 2, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
		// create a (also multisampled) renderbuffer object for depth and stencil attachments
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH * 2, SCR_HEIGHT * 2);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		// configure second post-processing framebuffer
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// create a color attachment texture
	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH * 2, SCR_HEIGHT * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer
	
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
		
//		glm::mat4 model = glm::mat4(1.0f);
//		glm::mat4 projection;
//		glm::mat4 view;
//		
//		shader.use();
//		glBindVertexArray(cubeVAO);
//		view = camera.GetViewMatrix();
//		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
//		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
//		glBindBuffer(GL_UNIFORM_BUFFER, 0);
//		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
//		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
//		glBindBuffer(GL_UNIFORM_BUFFER, 0);
//		shader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);

			// 1. draw scene as normal in multisampled buffers
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
			// set transformation matrices		
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		shader.setMat4("projection", projection);
		shader.setMat4("view", camera.GetViewMatrix());
		shader.setMat4("model", glm::mat4(1.0f));
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
			// 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2, 0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
			// 3. now render quad with scene's visuals as its texture image
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		
			// draw Screen quad
		screenShader.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture); // use the now resolved color attachment as the quad's texture
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
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
