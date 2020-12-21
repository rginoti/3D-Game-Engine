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

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


int main() 
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		// Need this line to activate core-profile mode.
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
	
//	Shader ourShader1("../../../libraries/shaders/vertexShaderSrc.vs", "../../../libraries/shaders/fragmentShaderSrc.fs");
	Shader ourShader1("/Users/ronaldginoti/Documents/C++/libraries/shaders/vertex3.5.txt", "/Users/ronaldginoti/Documents/C++/libraries/shaders/fragment3.5.txt");
	
	float vertices[] =
	{
			// Positions		// Colors
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 	0.0f, 0.0f, 1.0f
	};
	
	float texCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		.5f, 1.0f
	};
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	
	bool SwitchBetweenModes = 0;
	bool mode = 0;
	float hOffset = 0.0f;
	bool negative = 0;
	
	
	while(!glfwWindowShouldClose(window))
	{
			// Input
		processInput(window);
		if(mode == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		
		ourShader1.use();
		ourShader1.setFloat("hOffset", hOffset);
		
			// Rendering commands here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
//		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
			//		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
			//		glBindVertexArray(0); // no need to unbind it every time
		
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
		if(vertices[1] + hOffset >= 1.0f || vertices[1 + 6] + hOffset >= 1.0f || vertices[1 + 12] + hOffset >= 1.0f)
		{
			negative = 1;
		}
		else if(vertices[1] + hOffset <= -1.0f || vertices[1 + 6] + hOffset <= -1.0f || vertices[1 + 12] + hOffset <= -1.0f)
		{
			negative = 0;
		}
		
		if(negative)
		{
			hOffset -= .001f;
		}
		else
		{
			hOffset += .001f;
		}
	}
	
		//		optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
		//	glDeleteBuffers(1, &EBO);
	
		//		glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	
	
	return 0;
}
