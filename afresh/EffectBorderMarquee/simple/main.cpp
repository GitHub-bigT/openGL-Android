#include <GLAD\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
//#include <common.h>


int main()
{
#pragma region ≈‰÷√
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(512, 512, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {glViewport(0, 0, w, h); });
#pragma endregion

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float vertices[] = {
		-0.5,-0.5,	0,		0,0,
		0.5,-0.5,	0,		1,0,
		0.5,0.5,	0,		1,1,
		-0.5,0.5,	0,		0,1,
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned char colors[] = {
		255,0,0,255,	0,255,0,255,	0,0,255,255,	255,255,0,255
	};

	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_1D, tex);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glBindTexture(GL_TEXTURE_1D, 0);


	Shader shader = Shader("100.vert", "100.frag");
	shader.use();
	shader.setInt("repeats", 4);
	shader.setInt("pic_width", 256);
	shader.setInt("pic_height", 256);
	shader.setInt("bell", 20);


	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		shader.setFloat("t", -float(glfwGetTime() / 10));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}