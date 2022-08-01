#pragma once

#include <string>
#include <filesystem>
#include <glm/glm.hpp>

class Shader {
	int id = -1;
public:
	Shader(const std::string& vsource, const std::string& fsource);
	Shader(const std::filesystem::path& vsource, const std::filesystem::path& fsource);
	~Shader();

	operator int() const;

	void use() const;

	int getId() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, float x, float y) const;
  void setVec3(const std::string& name, const glm::vec3& v) const;
  void setVec4(const std::string& name, const glm::vec4& v) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setTexture(const std::string& name, int location) const;
};
