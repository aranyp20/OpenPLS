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
#include "Gui.h"
#include "Animation.h"
#include "AnimationRunner.h"



class Program
{
	
	Program();
	Program(const Program&) = delete;
	Program(Program&&) = delete;
	Program& operator=(const Program&) = delete;
	Program& operator=(Program&&) = delete;


	GLFWwindow* window;

	static Surface* surface;
	static GUI::Hud* hud;

	static unsigned int surfaceWidth;
	static unsigned int surfaceHeight;
	static unsigned int surfaceStartingX;
	static unsigned int surfaceStartingY;

	static unsigned int windowWidthR;
	static unsigned int windowHeightR;

public:
	
	
	static Program& GetInstance();

	bool Init();
	void WorldInit();
	void Run();

	static unsigned int SurfaceWidth();
	static unsigned int SurfaceHeight();
	static unsigned int SurfaceStartingX();
	static unsigned int SurfaceStartingY();

	static unsigned int WindowWidthR();
	static unsigned int WindowHeightR();

	static Surface* GetSurface();



};

#endif