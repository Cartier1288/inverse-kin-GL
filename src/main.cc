#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <math.h>

#include "shader.h"

GLFWwindow* window = nullptr;
Shader* baseShader = nullptr;


void terminate(int code) {

	glfwDestroyWindow(window);
	glfwTerminate();

	delete baseShader;

	exit(code);
}

void glfw_error(int error, const char* description) {
	std::cout << description << std::endl;
}

void glfw_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		terminate(0);
	}
}

int main(void) {

	if(!glfwInit()) {
		std::cerr << "failed GLFW initialization" << std::endl;
		return 1;
	}

	glfwSetErrorCallback(glfw_error);

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "title :)", NULL, NULL);
	if(!window) {
		std::cout << "failed window initialization" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, glfw_input);
	glfwShowWindow(window);
	glfwFocusWindow(window);


	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "failed GLAD GL initialization" << std::endl;
		return 1;
	}
	std::cout << "OpenGL " << GLVersion.major << GLVersion.minor << std::endl;


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0,0,width,height);

	glfwSwapInterval(1);

    std::cout << std::filesystem::current_path() << std::endl;
	std::cout << "ROOT_DIR: " << ROOT_DIR << std::endl;
	std::cout << "RESOURCE_DIR: " << RESOURCE_DIR << std::endl;
	std::cout << "SHADER_DIR: " << SHADER_DIR << std::endl;



	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);



	baseShader = new Shader { SHADER_DIR / std::filesystem::path("base.vert"), SHADER_DIR / std::filesystem::path("base.frag") };


	/* -- vertices and their buffers ------------------- */
	float vertices[] = {
		-1.0, -1.0,  0.0, 0.0,
		-1.0,  1.0,  0.0, 1.0,
		 1.0,  1.0,  1.0, 1.0,
		 1.0, -1.0,  1.0, 0.0
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	/* -- textures ------------------------------------- */
	unsigned int proctext;

	glGenTextures(1, &proctext);
	glBindTexture(GL_TEXTURE_2D, proctext);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	const unsigned int texwidth = height, texheight = height, nchan = 4;

	unsigned char *data = new unsigned char[texwidth * texheight * nchan];

	for (int i = 0; i < texwidth; i++) {
		const int x = i - (texwidth/2);
		for(int j = 0; j < texwidth; j++) {
			const int pos = (i * texwidth)*nchan + j*nchan;

			const int y = j - (texheight/2);
			const float h = x*x + y*y;
			const int inside = sqrt(h) < (texwidth/2) * abs(sin(atan2(y,x)*4.0));

			data[pos + 0] = 255 * inside * sqrt(h)/texwidth;
			data[pos + 1] = 255 * inside * sqrt(h)/texwidth;
			data[pos + 2] = 255 * inside * sqrt(h)/texwidth;
			data[pos + 3] = 255 * inside * sqrt(h)/texwidth;

			data[pos + 0] += 255 * inside * h;
			data[pos + 1] += 255 * inside * h;
			data[pos + 2] += 255 * inside * h;
			data[pos + 3] += 255 * inside * h;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] data;

	baseShader->use();
	baseShader->setShader("texture2d", 0);

	while(!glfwWindowShouldClose(window)) {

		float time = glfwGetTime();

		glClearColor (0.5921, 0.5961, 0.5255, 1.0);
		glClear (GL_COLOR_BUFFER_BIT);
		glColor3f (1.0, 1.0, 1.0);

		baseShader->use();
		baseShader->setFloat("time", time);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, proctext);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	terminate(0);

	return 0;
}
