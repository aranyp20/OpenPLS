#include <iostream>
#include "Program.h"
#include "InputManager.h"
#include "Factory.h"


vec2 InputManager::mousePos1 = vec2(0, 0);
vec2 InputManager::mousePos2 = vec2(0, 0);
std::vector<InputProcessor*> InputManager::inputProcessors = std::vector<InputProcessor*>();
InputBindable* InputManager::lastBind = NULL;
Factory* InputManager::factory = NULL;

void InputManager::SetCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (lastBind == NULL && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		for(InputProcessor* IP : inputProcessors)
		{


			InputAnswer IA = IP->ProcessMouseClick();
			if (IA.react == InputAnswer::ReactionType::CREATE) {
				factory->CreateOperation(IA.creation);
				return;
			}
			else if (IA.react == InputAnswer::ReactionType::PROCESSED) {
				return;
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (lastBind != NULL) {
			lastBind->Release();
			lastBind = NULL;
			return;
		}
	}
        
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (lastBind == NULL && action == GLFW_PRESS) {
		for(InputProcessor* IP : inputProcessors)
		{


			InputAnswer IA = IP->ProcessKey(key);
			if (IA.react == InputAnswer::ReactionType::CREATE) {
				factory->CreateOperation(IA.creation);
				return;
			}
			else if (IA.react == InputAnswer::ReactionType::PROCESSED) {
				return;
			}
		}
	}
	else if (action == GLFW_RELEASE) {
		if (lastBind != NULL) {
			lastBind->Release();
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

InputAnswer::InputAnswer(ReactionType _react, OperationType _creation) :react(_react),creation(_creation)
{
}
InputAnswer::InputAnswer(ReactionType _react) : react(_react){}

InputAnswer::InputAnswer() : react(InputAnswer::ReactionType::IGNORED){}

vec2 InputManager::GetMousePos1()
{
	return mousePos1;
}
vec2 InputManager::GetMousePos2()
{
	return mousePos2;
}

vec2 InputManager::ChangeInput(const vec2& v, bool toSurface)
{
	if(toSurface){
		
		float forSurfaceX = v.x-Program::SurfaceStartingX();
		float forSurfaceY = v.y-Program::SurfaceStartingY();

		float cX = 2.0f * forSurfaceX / Program::SurfaceWidth() - 1;
		float cY = 1 - 2.0f * forSurfaceY / Program::SurfaceHeight();
		
		return vec2(cX, cY);
	}else{
		float cX = 2.0f * v.x / Program::WindowWidthR() - 1;
		float cY = 1 - 2.0f * v.y/ Program::WindowHeightR();
		
		return vec2(cX, cY);
	}
}

void InputManager::AddIP(InputProcessor* ip) 
{
	inputProcessors.push_back(ip);


}

void InputManager::ChangeBind(InputBindable* newBind)
{
	if(lastBind!=NULL)lastBind->Release();
	lastBind=newBind;
}

Factory* InputManager::GetFactory(){return factory;}



