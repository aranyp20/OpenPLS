#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <vector>
#include "mymath.h"

class InputAnswer;

class InputCollection {
public:
	virtual InputAnswer ProcessKey(int key) = 0;
	InputAnswer ProcessMouseClick();
	InputAnswer ProcessMouseMotion();
};

class InputBindable {
public:
	virtual void Update() = 0;
};

struct InputAnswer {
	enum ReactionType {
		IGNORED,PROCESSED,BINDED
	};
	ReactionType react;
	InputBindable* bind;

	InputAnswer(ReactionType, InputBindable*);
};

class InputProcessor : public InputCollection{

public:
	
	
	
};


class InputManager {
	static std::vector<InputProcessor*> inputProcessors;
	static InputBindable* lastBind;

	static vec2 mousePos1;
	static vec2 mousePos2;


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

public:
	static void AddIP(InputProcessor*);

	static void SetCallbacks(GLFWwindow* window);
	static vec2 GetMousePos1();
	static vec2 GetMousePos2();

	static vec2 ChangeInput(const vec2&,bool toSurface = true);
};

class Operation : public InputBindable {


};

#endif