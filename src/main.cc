#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <filesystem>
#include <math.h>

#include "shader.h"
#include "ik.h"

GLFWwindow* window = nullptr;
Shader* baseShader = nullptr;
Shader* noiseShader = nullptr;
Shader* ikShader = nullptr;


void terminate(int code) {

	glfwDestroyWindow(window);
	glfwTerminate();

	delete baseShader;
	delete noiseShader;
  delete ikShader;

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
	noiseShader = new Shader { SHADER_DIR / std::filesystem::path("base.vert"), SHADER_DIR / std::filesystem::path("noise.frag") };
	ikShader = new Shader { SHADER_DIR / std::filesystem::path("ik.vert"), SHADER_DIR / std::filesystem::path("ik.frag") };


	/* -- vertices and their buffers ------------------- */
	float vertices[] = {
		-1.0, -1.0,  0.0,  0.0, 0.0,
		-1.0,  1.0,  0.0,  0.0, 1.0,
		 1.0,  1.0,  0.0,  1.0, 1.0,
		 1.0, -1.0,  0.0,  1.0, 0.0
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 1000.0f);
  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 model = glm::mat4(1.0f);
  //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
  baseShader->setMat4("projection", projection);
  baseShader->setMat4("view", view);
  baseShader->setMat4("model", model);
	baseShader->setTexture("texture2d", 0);

  noiseShader->use();
  noiseShader->setMat4("projection", projection);
  noiseShader->setMat4("view", view);
  noiseShader->setMat4("model", model);
	noiseShader->setTexture("texture2d", 0);

  ikShader->use();
  ikShader->setMat4("projection", projection);
  ikShader->setMat4("view", view);
  model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
  ikShader->setMat4("model", model);
	ikShader->setTexture("texture2d", 0);
  

  int rwidth, rheight;


  // inverse kinematics
  ik::chain c{std::vector<ik::joint>{
    ik::joint{ik::vec3{0.0, 0.0, 0.0}},
    ik::joint{ik::vec3{0.5, 0.5, 0.0}},
    ik::joint{ik::vec3{1.0, 0.45, 0.0}},
  }};
  
  std::cout << c.size() << std::endl;
  std::cout << c.length() << std::endl;

  ik::FABRIK f{c, ik::joint{ik::vec3{2.0, 2.0, 0.0}}};

  for(int i = 0; i < 100; i++) {
    //f.iterate();
    //std::cout << f.c << std::endl;
  }

	while(!glfwWindowShouldClose(window)) {

		float time = glfwGetTime();
    glfwGetWindowSize(window, &rwidth, &rheight);

		glClearColor (0.5921, 0.5961, 0.5255, 1.0);
		glClear (GL_COLOR_BUFFER_BIT);
		glColor3f (1.0, 1.0, 1.0);

		baseShader->use();
		baseShader->setFloat("time", time);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, proctext);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    noiseShader->use();
		noiseShader->setFloat("time", time);
    noiseShader->setVec2("WindowSize", rwidth, rheight);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    ikShader->use();
		ikShader->setFloat("time", time);

    for(size_t i = 0; i < f.c.size(); i++) {
      ikShader->setVec3("offset[" + std::to_string(i) + "]", glm::vec3{
        10 * f.c[i].pos.x,
        10 * f.c[i].pos.y,
        10 * f.c[i].pos.z
      });
    }

		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 3);
        

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	terminate(0);

	return 0;
}
