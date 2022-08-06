#include <iostream>
#include "Program.h"
#include "InputManager.h"


vec2 InputManager::mousePos1 = vec2(0, 0);
vec2 InputManager::mousePos2 = vec2(0, 0);
std::vector<InputProcessor*> InputManager::inputProcessors = std::vector<InputProcessor*>();
InputBindable* InputManager::lastBind = NULL;

void InputManager::SetCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (action == GLFW_PRESS) {
		for(InputProcessor* IP : inputProcessors)
		{


			InputAnswer IA = IP->ProcessKey(key);
			if (IA.react == InputAnswer::ReactionType::BINDED) {
				lastBind = IA.bind;
				return;
			}
			else if (IA.react == InputAnswer::ReactionType::PROCESSED) {
				return;
			}
		}
	}
	else if (action == GLFW_RELEASE) {
		if (lastBind != NULL) {
			delete lastBind;
			lastBind = NULL;
			return;
		}
	}
		
}

void InputManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mousePos1 = mousePos2;

	mousePos2.x = xpos;
	mousePos2.y = ypos;
	if (lastBind != NULL) {
		lastBind->Update();
	}
}

InputAnswer::InputAnswer(ReactionType _react, InputBindable* _bind = NULL) :react(_react),bind(_bind)
{
}

vec2 InputManager::GetMousePos1()
{
	return mousePos1;
}
vec2 InputManager::GetMousePos2()
{
	return mousePos2;
}

vec2 InputManager::ChangeInput(const vec2& v)
{
	float cX = 2.0f * v.x / Program::GetInstance().WindowWidth() - 1;
	float cY = 1 - 2.0f * v.y / Program::GetInstance().WindowHeight();
	return vec2(cX, cY);
}

void InputManager::AddIP(InputProcessor* ip) 
{
	inputProcessors.push_back(ip);


}



