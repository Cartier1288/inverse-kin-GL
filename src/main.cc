#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"

GLFWwindow* window = nullptr;


void terminate(int code) {
	glfwDestroyWindow(window);
	glfwTerminate();

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

	while(!glfwWindowShouldClose(window)) {
		glClearColor (0.5921, 0.5961, 0.5255, 1.0);
		glClear (GL_COLOR_BUFFER_BIT);
		glColor3f (1.0, 1.0, 1.0);
		glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex3f (0.25, 0.25, 0.0);
			glVertex3f (0.75, 0.25, 0.0);
			glVertex3f (0.75, 0.75, 0.0);
			glVertex3f (0.25, 0.75, 0.0);
		glEnd();
		//glFlush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	terminate(0);

	return 0;
}
