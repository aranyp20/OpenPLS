#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <vector>
#include "mymath.h"

class InputAnswer;



class InputBindable {
public:
	virtual void Update() = 0;
	virtual void Release(){}
};

struct InputAnswer {
	enum ReactionType {
		IGNORED,PROCESSED,BINDED
	};
	ReactionType react;
	InputBindable* bind;

	InputAnswer(ReactionType, InputBindable*);
};

class InputProcessor{

public:
	inline virtual InputAnswer ProcessKey(int key){return InputAnswer(InputAnswer::ReactionType::IGNORED,NULL);}
	inline virtual InputAnswer ProcessMouseClick(){return InputAnswer(InputAnswer::ReactionType::IGNORED,NULL);}
	inline virtual InputAnswer ProcessMouseMotion(){return InputAnswer(InputAnswer::ReactionType::IGNORED,NULL);}
	
	
};


class InputManager {
	static std::vector<InputProcessor*> inputProcessors;
	static InputBindable* lastBind;

	static vec2 mousePos1;
	static vec2 mousePos2;


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
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