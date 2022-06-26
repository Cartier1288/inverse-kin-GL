#pragma once

#include <string>

class Shader {
	int id = -1;
public:
	Shader(const std::string& vsource, const std::string& fsource);
	~Shader();

	operator int() const;

	void use() const;

	int getId() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};