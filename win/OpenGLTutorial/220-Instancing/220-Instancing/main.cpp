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

	// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


glm::vec3 pointLightPositions[] = {
	glm::vec3(50.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f,  50.0f),
	glm::vec3(-50.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f,  -50.0f),
};

glm::vec3 pointLightColors1[] = {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0, 1.0),
	glm::vec3(1.0f, 1.0f, 1.0f)
};
//float quadVertices[] = {
//		// positions     // colors
//	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//	0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
//	
//	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//	0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
//	0.05f,  0.05f,  0.0f, 1.0f, 1.0f		    		
//};

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
//	glEnable(GL_PROGRAM_POINT_SIZE);
	
		// build and compile shaders
		// -------------------------
	Shader shader("../../../libraries/shaders/vertex22.glsl", "../../../libraries/shaders/fragment22.glsl");
	Shader instanceShader("../../../libraries/shaders/vertex22Instance.glsl", "../../../libraries/shaders/fragment22Instance.glsl");
	Model planetModel("../../../assets/planet/planet.obj");
	Model rockModel("../../../assets/rock/rock.obj");

		// quad VAO
//	unsigned int quadVAO, quadVBO;
//	glGenVertexArrays(1, &quadVAO);
//	glGenBuffers(1, &quadVBO);
//	glBindVertexArray(quadVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), NULL, GL_STATIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), &quadVertices);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	
		// Real example of uniform block setup
	unsigned int matricesIndexes = glGetUniformBlockIndex(shader.ID, "Matrices");
	glUniformBlockBinding(shader.ID, matricesIndexes, 0);
	unsigned int matricesUBO;
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));
	
//	glm::vec2 translations[100];
//	int index = 0;
//	float offset = 0.1f;
//	for(int y = -10; y < 10; y += 2)
//	{
//		for(int x = -10; x < 10; x += 2)
//		{
//			glm::vec2 translation;
//			translation.x = (float)x / 10.0f + offset;
//			translation.y = (float)y / 10.0f + offset;
//			translations[index++] = translation;
//		}
//	}
//	unsigned int instanceVBO;
//	glGenBuffers(1, &instanceVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);	
//	glVertexAttribDivisor(2, 1);
	
	unsigned int amount = 10000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());	// Initialize random seed.
	float radius = 50;
	float offset = 2.5f;
	for(unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount* 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * .4f;	// keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		
			// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		
			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		
			// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
		// vertex Buffer Object
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	
	for(unsigned int i = 0; i < rockModel.meshes.size(); i++)
	{
		unsigned int VAO = rockModel.meshes[i].VAO;
		glBindVertexArray(VAO);
			// vertex Attributes
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3); 
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4); 
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5); 
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6); 
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
		
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		
		glBindVertexArray(0);
	}
	
//	shader.use();
//	for(unsigned int i = 0; i < 100; i++)
//	{
//		stringstream ss;
//		string index;
//		ss << i; 
//		index = ss.str(); 
//		shader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
//	}
	
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
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
//		shader.use();
//		glBindVertexArray(quadVAO);
//		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		
		shader.use();
		setPointLightUniform(4, pointLightPositions, pointLightColors1, &shader);
		model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		shader.setVec3("cameraPos", camera.Position);
		
		model = glm::translate(model, glm::vec3(0.0f, -3.00f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));	// it's a bit too big for our scene, so scale it down
		shader.setMat4("model", model);
		
		planetModel.Draw(shader);
		
			// draw meteorites
//		for(unsigned int i = 0; i < amount; i++)
//		{
//			shader.setMat4("model", modelMatrices[i]);
//			rockModel.Draw(shader);
//		}
		
			// draw meteorites
		instanceShader.use();
		for(unsigned int i = 0; i < rockModel.meshes.size(); i++)
		{
			glBindVertexArray(rockModel.meshes[i].VAO);
			glDrawElementsInstanced(
									GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
									);
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
