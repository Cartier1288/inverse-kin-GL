#include "shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

std::string readFile(std::string path) {
	std::ifstream f(path);
	if (f) {
		std::stringstream buffer;
		buffer << f.rdbuf();

		return buffer.str();
	}
	else {
		std::cout << "[ERROR] Couldn't load file at: " << path << std::endl;
		return "";
	}
}

void getShaderStatus(std::string name, int id) {
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "[ERROR] SHADER->" << name << "->COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

Shader::Shader(const std::string& vsource, const std::string& fsource) {
	std::string vexStr = readFile(vsource);
	const char* vex = vexStr.c_str();
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vex, NULL);
	glCompileShader(vertexShader);

	getShaderStatus("VERTEX", vertexShader);

	std::string fragStr = readFile(fsource);
	const char* frag = fragStr.c_str();
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &frag, NULL);
	glCompileShader(fragmentShader);

	getShaderStatus("FRAGMENT", fragmentShader);

	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);
	glLinkProgram(this->id);

	int success;
	char infoLog[512];
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "[ERROR] SHADER->PROGRAM->LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


Shader::Shader(const std::filesystem::path& vsource, const std::filesystem::path& fsource)
    : Shader::Shader(vsource.string(), fsource.string()) { }

Shader::~Shader() {
	glDeleteProgram(this->id);
}

Shader::operator int() const {
	return this->id;
}

void Shader::use() const {
	glUseProgram(this->id);
}

int Shader::getId() const {
	return id;
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(this->id, name.c_str()), (float)value);
}

void Shader::setVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
  glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &v[0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setTexture(const std::string& name, int location) const {
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)location);
}
