#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Renderer.h"
#include "VBO.h"
#include "IBO.h"
#include "VAO.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class Program
{
	
	Program();
	Program(const Program&) = delete;
	Program(Program&&) = delete;
	Program& operator=(const Program&) = delete;
	Program& operator=(Program&&) = delete;


	GLFWwindow* window;

	static unsigned int windowWidth;
	static unsigned int windowHeight;

public:
	
	
	static Program& GetInstance();

	bool Init();
	void Run();

	unsigned int WindowWidth();
	unsigned int WindowHeight();

};