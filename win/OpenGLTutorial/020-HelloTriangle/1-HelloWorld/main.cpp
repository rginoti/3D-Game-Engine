//
//  main.cpp
//  OpenGL Tutorial Chp.1
//
//  Created by Ronald  Ginoti on 1/17/19.
//  Copyright © 2019 Ronald Ginoti. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
//#include "../../../libraries/include/glad/glad.h"
#include <GLFW/glfw3.h>
//#include "../../../libraries/include/GLFW/glfw3.h"


const char *vertexShaderSource = 	"#version 330 core\n"
									"layout (location = 0) in vec3 aPos;\n"
									"void main()\n"
									"{\n"
									"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
									"}\0";

const char *fragmentShaderSource = 	"#version 410 core\n"
									"out vec4 FragColor;\n"
									"void main()"
									"{\n"
									"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									"}\0";

const char *fragmentShaderSource2 =	"#version 410 core\n"
									"out vec4 FragColor;\n"
									"void main()"
									"{\n"
									"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
									"}\0";


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
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	success = NULL;
	infoLog[0] = '\0';
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	success = NULL;
	infoLog[0] = '\0';
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	success = NULL;
	infoLog[0] = '\0';
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
		// Deleted vertexShader too early I think (coming from lesson 21).
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	success = NULL;
	infoLog[0] = '\0';
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if(!success) 
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
		// Deleting wrong fragmentShader (coming from lesson 21).
	glDeleteShader(fragmentShader);
	
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	
//	float vertices2[] =
//	{
//			// First 3 are the first triangle.
//			// Last 3 are the second triangle.
//		.9, .7, 0,
//		.2, .7, 0,
//		.4, -.2, 0,
//		-.2, -.3, 0
//	};
	
	float vertices3[] =
	{
			// First Triangle
		-.7, -.9, 0,
		-.2, -.9, 0,
		-.45, -.2, 0,
		
			// Second Triangle
		.7, .9, 0,
		.2, .9, 0,
		.45, .2, 0
	};
	
//	unsigned int indices[] =
//	{
//		0, 1, 2,
//		1, 2, 3
//	};
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	
//	unsigned int EBO;
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		// Allowed to do it here because the VAO is not active.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	bool SwitchBetweenModes = 0;
	bool mode = 0;
	
	
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
		
			// Rendering commands here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0); // no need to unbind it every time
		
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
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
	}
	
//		optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
	
//		glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	
	
	return 0;
}
