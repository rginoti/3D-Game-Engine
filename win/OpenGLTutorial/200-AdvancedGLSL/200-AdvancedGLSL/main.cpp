

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


	// set up vertex data
	// ------------------------------------------------------------------
float cubePositions[] = {
		// Back face
	-0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,           
	0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f, 
	-0.5f,  0.5f, -0.5f, 
		// Front face
	-0.5f, -0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f, 
	-0.5f, -0.5f,  0.5f, 
		// Left face
	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f, 
		// Right face
	0.5f,  0.5f,  0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,           
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,       
		// Bottom face
	-0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f, 
	-0.5f, -0.5f, -0.5f, 
		// Top face
	-0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f, 
	-0.5f,  0.5f,  0.5f, 
};
float cubeNormals[] = {
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	
	  0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,
	
	  0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,
	
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f
};
float planePositions[] = {
		// positions
	5.0f, -0.5f,  5.0f,
	-5.0f, -0.5f,  5.0f,
	-5.0f, -0.5f, -5.0f,
	
	5.0f, -0.5f,  5.0f,
	-5.0f, -0.5f, -5.0f,
	5.0f, -0.5f, -5.0f
};
float planeTexCoords[] = {
		// texture Coords 
	2.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 2.0f,
	
	2.0f, 0.0f,
	0.0f, 2.0f,
	2.0f, 2.0f
};
float quadPositions[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions
	-1.0f,  1.0f, 
	-1.0f, -1.0f, 
	1.0f, -1.0f,  
	
	-1.0f,  1.0f, 
	1.0f, -1.0f,  
	1.0f,  1.0f,  
};
float quadTexCoords[] = {
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};
float skyboxVertices[] = {
		// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	
	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};
vector<std::string> faces
{
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
};
std::string skyBoxFilePath = "../../../assets/skybox/";

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

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load((skyBoxFilePath + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
						 );
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return textureID;
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
	Shader shader("../../../libraries/shaders/vertex20.glsl", "../../../libraries/shaders/fragment20.glsl");

		// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions) /*+ sizeof(cubeNormals)*/, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubePositions), &cubePositions);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubePositions), sizeof(cubeNormals), &cubeNormals);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(cubePositions)));
	
		// How to setup uniform buffer object.
			// Fake example.
//	unsigned int uboExampleBlock;
//	glGenBuffers(1, &uboExampleBlock);
//	glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
//	glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//		// Binding the uniform block is unnecessary in version 4.2 if
//			// layout(std140, binding = 2) uniform Lights { ... }; is placed in the shader.
//	unsigned int exampleBlockIndex = glGetUniformBlockIndex(shader.ID, "Matrices");
//	glUniformBlockBinding(shader.ID, exampleBlockIndex, 2);
//		// This binds the buffer we have generated.
//	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock); 
//		// or
//		// Can use this version to send different ranges from the buffer to separate uniform blocks.
////	glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);
//	glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
//	int b = true; // bools in GLSL are represented as 4 bytes, so we store it in an integer
//	glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b); 
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
		// Real example of uniform block setup
	unsigned int matricesIndex = glGetUniformBlockIndex(shader.ID, "Matrices");
	glUniformBlockBinding(shader.ID, matricesIndex, 0);
	unsigned int matricesUBO;
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));
	
		// draw as wireframe
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
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
		
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		
		shader.use();
		model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
//		shader.setMat4("view", view);
//		shader.setMat4("projection", projection);
//		shader.setVec3("cameraPos", camera.Position);
			// cubes
		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
//		glDrawArrays(GL_POINTS, 0, 36);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	
	glfwTerminate();
	return 0;
}
