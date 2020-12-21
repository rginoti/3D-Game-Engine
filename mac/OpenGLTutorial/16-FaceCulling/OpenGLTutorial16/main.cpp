

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SHADER/shader.h>
#include <CAMERA/camera.h>
#include <MODEL/model.h>

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path, bool alphaChannel);

	// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

	// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
		// This will cull front facing triangles.
	glCullFace(GL_BACK);
		// This will make clockwise order front facing.
	glFrontFace(GL_CW);
	
		// build and compile shaders
		// -------------------------
	Shader shader("/Users/ronaldginoti/Documents/C++/libraries/shaders/vertex15.glsl", "/Users/ronaldginoti/Documents/C++/libraries/shaders/fragment15.glsl");
	
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
//	float cubeVertices[] = {
//			// Back face
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//			// Front face
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//			// Left face
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//			// Right face
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
//		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
//			// Bottom face
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//			// Top face
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
//	};
	float cubeVertices[] = {
			// Back face
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			// Left face
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			// Right face
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			// Top face
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
//	float planeVertices[] = {
//			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
//		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//		
//		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
//		5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
//	};
	float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
	};
	float transparentVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
	windows.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
	windows.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
	windows.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
	windows.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));
	
		// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
		// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
		// Grass VAO
	unsigned int vegetationVAO, vegetationVBO;
	glGenVertexArrays(1, &vegetationVAO);
	glGenBuffers(1, &vegetationVBO);
	glBindVertexArray(vegetationVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	
	
		// load textures
		// -------------
	stbi_set_flip_vertically_on_load(true);
	unsigned int cubeTexture  = loadTexture("/Users/ronaldginoti/Documents/C++/assets/marble.jpg", 0);
	unsigned int floorTexture = loadTexture("/Users/ronaldginoti/Documents/C++/assets/metal.png", 0);
	unsigned int glassTexture = loadTexture("/Users/ronaldginoti/Documents/C++/assets/transparent_window.png", 1);
	
		// shader configuration
		// --------------------
	shader.use();
	shader.setInt("texture1", 0);
	
		// render loop
		// -----------
	while(!glfwWindowShouldClose(window))
	{
			// per-frame time logic
			// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
			// input
			// -----
		processInput(window);
		
			// sort the transparent windows before rendering
			// ---------------------------------------------
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(camera.Position - windows[i]);
			sorted[distance] = windows[i];
		}
		
			// render
			// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		
			// floor
		shader.use();
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		
			// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture); 	
		model = glm::translate(model, glm::vec3(-1.0f,  0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3( 2.0f,  0.0f,  0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
			// Windows
		glBindVertexArray(vegetationVAO);
		glBindTexture(GL_TEXTURE_2D, glassTexture);  
//		for(unsigned int i = 0; i < windows.size(); i++) 
//		{
//			model = glm::mat4(1.0f);
//			model = glm::translate(model, windows[i]);				
//			shader.setMat4("model", model);
//			glDrawArrays(GL_TRIANGLES, 0, 6);
//		}
		for(std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) 
		{
			model = glm::mat4(1.0f);
		 		// I think second in it->second is refering to the second type in the map (<first = float, second = glm::vec3>).
			model = glm::translate(model, it->second);				
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	
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
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}

	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
unsigned int loadTexture(char const *path, bool alphaChannel)
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
		
		if(alphaChannel)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}

