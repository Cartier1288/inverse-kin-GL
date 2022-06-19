#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void glfw_error(int error, const char* description) {
	std::cout << description << std::endl;
}

int main(void) {

	if(!glfwInit()) {
		std::cerr << "failed GLFW initialization" << std::endl;
		return 1;
	}

	glfwSetErrorCallback(glfw_error);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "title :)", NULL, NULL);
	if(!window) {
		std::cout << "failed window initialization" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);


	if(!gladLoadGL()) {
		std::cerr << "failed GLAD GL initialization" << std::endl;
		return 1;
	}
	std::cout << "OpenGL " << GLVersion.major << GLVersion.minor << std::endl;


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0,0,width,height);



	while(!glfwWindowShouldClose(window)) {
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glClear (GL_COLOR_BUFFER_BIT);
		glColor3f (1.0, 1.0, 1.0);
		glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex3f (0.25, 0.25, 0.0);
			glVertex3f (0.75, 0.25, 0.0);
			glVertex3f (0.75, 0.75, 0.0);
			glVertex3f (0.25, 0.75, 0.0);
		glEnd();
		glFlush();
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
