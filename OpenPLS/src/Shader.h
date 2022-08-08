#ifndef SHADER_H
#define SHADER_H

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
	struct Data{

	};



	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void SetUniform(const std::string& name, const float f);
	void SetUniform(const std::string& name, const vec4& v);
	void SetUniform(const std::string& name, const vec3& v);
	void SetUniform(const std::string& name, const mat4& m);
	void SetUniform(const std::string& name, const Material& material);
	void SetUniform(const std::string& name, const Light& light);

	//virtual void PrepareForRendering(Shader::Data) = 0;
private:
	unsigned int getUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	bool CompileErrorHandler(unsigned int shaderID) const;
};

class GouraudShader : public Shader{
	public:
	//cacheeles jo lenne
	struct Data : public Shader::Data{
	

		Light* light;
		vec3 wEye;
		mat4 M; mat4 V; mat4 P; mat4 Minv;
		Data(Light* _light, vec3 _wEye, mat4 _M, mat4 _V, mat4 _P, mat4 _Minv): light(_light),wEye(_wEye),M(_M),V(_V),P(_P),Minv(_Minv){}
	};

	void PrepareForRendering(GouraudShader::Data);
	GouraudShader() : Shader("../shaders/GouraudShader.shader"){}
};

class NormalShader : public Shader{
	public:
		struct Data : public Shader::Data{

		mat4 M; mat4 V; mat4 P; 
		Data(mat4 _M, mat4 _V, mat4 _P): M(_M),V(_V),P(_P){}
	};
	void PrepareForRendering(NormalShader::Data);
	NormalShader() : Shader("../shaders/Shader1.shader"){}
};


#endif