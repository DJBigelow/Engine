#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	const void setBool(const std::string& name, bool value);
	const void setInt(const std::string& name, int value);


private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
