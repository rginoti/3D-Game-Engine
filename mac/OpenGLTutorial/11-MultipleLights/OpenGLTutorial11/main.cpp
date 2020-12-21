#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SHADER/shader.h>
#include <CAMERA/camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);

	// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

	// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};


glm::vec3 pointLightColors1[] = {
	glm::vec3(1.0f, 0.6f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0, 0.0),
	glm::vec3(0.2f, 0.2f, 1.0f)
};

glm::vec3 pointLightColors2[] = {
	glm::vec3(0.2f, 0.2f, 0.6f),
	glm::vec3(0.3f, 0.3f, 0.7f),
	glm::vec3(0.0f, 0.0f, 0.3f),
	glm::vec3(0.4f, 0.4f, 0.4f)
};

glm::vec3 pointLightColors3[] = {
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.3f, 0.1f, 0.1f)
};

glm::vec3 pointLightColors4[] = {
	glm::vec3(0.4f, 0.7f, 0.1f),
	glm::vec3(0.4f, 0.7f, 0.1f),
	glm::vec3(0.4f, 0.7f, 0.1f),
	glm::vec3(0.4f, 0.7f, 0.1f)
};

void setPointLightUniform(uint lightCount, glm::vec3 *lightPosition, glm::vec3 *lightColor, Shader *shader);

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
	
		// build and compile our shader zprogram
		// ------------------------------------
	Shader lightingShader("/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/vertex11.glsl", "/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/fragment11.glsl");
	Shader lampShader("/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/vertex8Lamp.txt", "/Users/ronaldginoti/Documents/C++/OpenGL/libraries/shaders/fragment8Lamp.txt");
	
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
	
	
	float vertices[] = {
			// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  	 0.0f,  0.0f, -1.0f,  	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  	 0.0f,  0.0f, -1.0f,  	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  	 0.0f,  0.0f, -1.0f,  	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  	 0.0f,  0.0f, -1.0f,  	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  	 0.0f,  0.0f, -1.0f,  	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  	 0.0f,  0.0f, -1.0f,  	0.0f, 0.0f,
				 
		-0.5f, -0.5f,  0.5f,  	 0.0f,  0.0f, 1.0f,   	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  	 0.0f,  0.0f, 1.0f,   	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  	 0.0f,  0.0f, 1.0f,   	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  	 0.0f,  0.0f, 1.0f,   	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  	 0.0f,  0.0f, 1.0f,   	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  	 0.0f,  0.0f, 1.0f,   	0.0f, 0.0f,
		
		-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,  	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,  	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
			
		 0.5f,  0.5f,  0.5f,  	 1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  	 1.0f,  0.0f,  0.0f,  	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  	 1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  	 1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  	 1.0f,  0.0f,  0.0f,  	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  	 1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
			 	
		-0.5f, -0.5f, -0.5f,  	 0.0f, -1.0f,  0.0f,  	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  	 0.0f, -1.0f,  0.0f,  	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  	 0.0f, -1.0f,  0.0f,  	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  	 0.0f, -1.0f,  0.0f,  	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  	 0.0f, -1.0f,  0.0f,  	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  	 0.0f, -1.0f,  0.0f,  	0.0f, 1.0f,
			 	
		-0.5f,  0.5f, -0.5f,  	 0.0f,  1.0f,  0.0f,  	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  	 0.0f,  1.0f,  0.0f,  	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  	 0.0f,  1.0f,  0.0f,  	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  	 0.0f,  1.0f,  0.0f,  	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  	 0.0f,  1.0f,  0.0f,  	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  	 0.0f,  1.0f,  0.0f,  	0.0f, 1.0f
	};
	
		// world space positions of our cubes
	glm::vec3 cubePositions[] = {
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
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(cubeVAO);
	
		// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
		// Texture Coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	
	stbi_set_flip_vertically_on_load(true);
	unsigned int diffuseMap = loadTexture("/Users/ronaldginoti/Documents/C++/OpenGL/assets/chest.png");
	unsigned int specularMap = loadTexture("/Users/ronaldginoti/Documents/C++/OpenGL/assets/chest_specular.png");
	unsigned int emissionMap = loadTexture("/Users/ronaldginoti/Documents/C++/OpenGL/assets/matrix.jpg");
	
	int frameCount = 0;
	float subtractTime = 0;
	
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	
	
		// render loop
		// -----------
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
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
//		glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		lightingShader.use();
			// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		
			// point light 1
		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].ambient"), pointLightColors3[0].x * 0.1,  pointLightColors3[0].y * 0.1,  pointLightColors3[0].z * 0.1);
		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].diffuse"), pointLightColors3[0].x,  pointLightColors3[0].y,  pointLightColors3[0].z); 
		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].specular"), pointLightColors3[0].x,  pointLightColors3[0].y,  pointLightColors3[0].z);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].constant"), 1.0f);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].linear"), 0.09);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].quadratic"), 0.032);		
//			// Point light 2
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].ambient"), pointLightColors3[1].x * 0.1,  pointLightColors3[1].y * 0.1,  pointLightColors3[1].z * 0.1);		
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].diffuse"), pointLightColors3[1].x,  pointLightColors3[1].y,  pointLightColors3[1].z); 
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].specular"), pointLightColors3[1].x,  pointLightColors3[1].y,  pointLightColors3[1].z);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].constant"), 1.0f);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].linear"), 0.09);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].quadratic"), 0.032);		
//			// Point light 3
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);		
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].ambient"), pointLightColors3[2].x * 0.1,  pointLightColors3[2].y * 0.1,  pointLightColors3[2].z * 0.1);		
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].diffuse"), pointLightColors3[2].x,  pointLightColors3[2].y,  pointLightColors3[2].z); 
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].specular") ,pointLightColors3[2].x,  pointLightColors3[2].y,  pointLightColors3[2].z);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].constant"), 1.0f);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].linear"), 0.09);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].quadratic"), 0.032);		
//			// Point light 4
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);		
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].ambient"), pointLightColors3[3].x * 0.1,  pointLightColors3[3].y * 0.1,  pointLightColors3[3].z * 0.1);		
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].diffuse"), pointLightColors3[3].x,  pointLightColors3[3].y,  pointLightColors3[3].z); 
//		
//		glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].specular"), pointLightColors3[3].x,  pointLightColors3[3].y,  pointLightColors3[3].z);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].constant"), 1.0f);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].linear"), 0.09);
//		
//		glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].quadratic"), 0.032);
		
		setPointLightUniform(4, pointLightPositions, pointLightColors2, &lightingShader);
		
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		lightingShader.setVec3("spotLight.ambient",  0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("spotLight.diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.045f);
		lightingShader.setFloat("spotLight.quadratic", 0.0075f);
		
		lightingShader.setFloat("material.shininess", 32.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);
		
		lightingShader.setVec3("viewPos", camera.Position);
		
			// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);
		
			// camera/view transformation
					glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);
		
			// render boxes
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
				// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		lampShader.use();
		glBindVertexArray(lightVAO);
		
		for(int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(.2));
			lampShader.setMat4("projection", projection);
			lampShader.setMat4("view", view);
			lampShader.setMat4("model", model);
			lampShader.setVec3("Color", pointLightColors2[i]);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		frameCount++;
		if(time - subtractTime >= 1.0)
		{
			std::cout << "FPS: " << frameCount << std::endl;
			time = 0.0;
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
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
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
unsigned int loadTexture(char const * path)
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

	// Point Light Creation;
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
