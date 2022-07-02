#pragma once

#include <string>
#include <filesystem>

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
	void setShader(const std::string& name, int location) const;
};