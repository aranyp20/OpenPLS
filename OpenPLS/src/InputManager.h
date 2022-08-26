#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <vector>
#include "mymath.h"


class InputAnswer;
class Factory;


class InputBindable {
public:
	virtual void Update() = 0;
	virtual void Release(){}
};

struct InputAnswer {
	enum ReactionType {
		IGNORED,PROCESSED,CREATE
	};
	enum OperationType{
        VERT_ROTATE, VERT_SCALE, VERT_EXTRUDE, VERT_MOVE,
		VERT_SUBDIVIDE,
        CAMERA_MOVE, CAMERA_FOCUS_SET,
		BOX_SELECTION
    };
	ReactionType react;
	OperationType creation;

	InputAnswer(ReactionType, OperationType);
	InputAnswer(ReactionType);
	InputAnswer();
};

class InputProcessor{

public:
	inline virtual InputAnswer ProcessKey(int key){return InputAnswer();}
	inline virtual InputAnswer ProcessMouseClick(){return InputAnswer();}
	inline virtual InputAnswer ProcessMouseMotion(){return InputAnswer();}
	
	
};


class InputManager {
	static std::vector<InputProcessor*> inputProcessors;
	static InputBindable* lastBind;

	static vec2 mousePos1;
	static vec2 mousePos2;

	static Factory* factory;


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
public:
	static void AddIP(InputProcessor*);

	static void SetCallbacks(GLFWwindow* window);
	static vec2 GetMousePos1();
	static vec2 GetMousePos2();

	static vec2 ChangeInput(const vec2&,bool toSurface = true);

	static void ChangeBind(InputBindable*);

	static Factory* GetFactory();

	friend class Program;
};

class Operation : public InputBindable {


};

#endif