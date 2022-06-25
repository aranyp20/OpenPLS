#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

#include "mymath.h"
#include "WorldElements.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int ID;

public:
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void SetUniform(const std::string& name, const float f);
	void SetUniform(const std::string& name, const vec4& v);
	void SetUniform(const std::string& name, const vec3& v);
	void SetUniform(const std::string& name, const mat4& m);
	void SetUniform(const std::string& name, const Material& material);
	void SetUniform(const std::string& name, const Light& light);
private:
	unsigned int getUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	bool CompileErrorHandler(unsigned int shaderID) const;
};