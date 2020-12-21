	//
	//  main.cpp
	//  OpenGL Tutorial Chp.1
	//
	//  Created by Ronald  Ginoti on 1/17/19.
	//  Copyright © 2019 Ronald Ginoti. All rights reserved.
	//

#include <glad/glad.h>
	//#include "../../../libraries/include/glad/glad.h"
#include <GLFW/glfw3.h>
	//#include "../../../libraries/include/GLFW/glfw3.h"
#include <SHADER/shader.h>
#include <iostream>
#include <fstream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float mixRatio = 0.2f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window, Shader *shader1)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixRatio += 0.001f;
		shader1->setFloat("mixRatio", mixRatio);
	}
	else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixRatio -= 0.001f;
		shader1->setFloat("mixRatio", mixRatio);
	}
}


int main() 
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		// Need this line to activate core-profile mode on Mac.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
//	Shader ourShader1("/Users/ronaldginoti/Documents/C++/libraries/shaders/vertex4.txt", "/Users/ronaldginoti/Documents/C++/libraries/shaders/fragment4.txt");
	Shader ourShader1("../../../libraries/shaders/vertex4.txt", "../../../libraries/shaders/fragment4.txt");

	
	float vertices[] =
	{
			// Positions		// Colors
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-.5f, 0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		.5f, .5f, 0.0,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f
	};
	
	unsigned int indices[] =
	{
		0, 1, 2,
		1, 2, 3
	};
	
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../../../assets/container.jpg", &width, &height, &nrChannels, 0);
	
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(data);
	
	data = stbi_load("../../../assets/awesomeface.png", &width, &height, &nrChannels, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	
	bool SwitchBetweenModes = 0;
	bool mode = 0;
	
		// Must activate shader to set uniform.
	ourShader1.use();
		// Manually
	glUniform1i(glGetUniformLocation(ourShader1.ID, "texture1"), 0);
		// Or through shader class.
	ourShader1.setInt("texture2", 1);
	ourShader1.setFloat("mixRatio", mixRatio);
	
	
	while(!glfwWindowShouldClose(window))
	{
			// Input
		processInput(window, &ourShader1);
		if(mode == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		
			// Rendering commands here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader1.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
			// Check and call events and swap the buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
		if(SwitchBetweenModes == 1)
		{
			if(mode == 0)
			{
				mode = 1;
			}
			else
			{
				mode = 0;
			}
		}
	}
	
		//		optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
		//		glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	
	
	return 0;
}
