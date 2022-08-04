#ifndef PROGRAM_H
#define PROGRAM_H

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
#include "InputManager.h"
#include "Surface.h"


class Program
{
	
	Program();
	Program(const Program&) = delete;
	Program(Program&&) = delete;
	Program& operator=(const Program&) = delete;
	Program& operator=(Program&&) = delete;


	GLFWwindow* window;

	static Surface* surface;

	static unsigned int windowWidth;
	static unsigned int windowHeight;

public:
	
	
	static Program& GetInstance();

	bool Init();
	void Run();

	unsigned int WindowWidth();
	unsigned int WindowHeight();

};

#endif