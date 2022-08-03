#include "Surface.h"

InputAnswer Surface::ProcessKey(int key)
{
	if (key == GLFW_KEY_Q) {
		return InputAnswer(InputAnswer::ReactionType::BINDED, new OCameraMove(viewCamera));
	}

	
}

OCameraMove::OCameraMove(Camera* _camera) : camera(_camera)
{
	
	//camera->ReplaceEye(camera->GetEye() + vec3(1, 0, 0));
}

void OCameraMove::Update()
{
	vec2 tVec = InputManager::GetMousePos2() - InputManager::GetMousePos1();
	std::vector<vec3> wuv = camera->GetWUV();
	vec3 axis = wuv[2] * tVec.x + wuv[1] * tVec.y;
	vec3 newEye = camera->GetEye();
	newEye = newEye - camera->GetLookat();
	TransformPoint(newEye, RotationMatrix(tVec.length() / -700, axis));
	newEye = newEye + camera->GetLookat();
	camera->ReplaceEye(newEye);
	
}
