#include "Surface.h"

InputAnswer Surface::ProcessKey(int key)
{
	if (key == GLFW_KEY_Q) {
		return InputAnswer(InputAnswer::ReactionType::BINDED, new OCameraMove(viewCamera));
	}

	return InputAnswer(InputAnswer::ReactionType::IGNORED, NULL);
	
}


Toloka::Arrow::Arrow(vec3 _direction,vec3 _color, vec3 _center = vec3(0,0,0)) :direction(_direction), color(_color), center(_center)
{
	
}


void Toloka::Arrow::Update() 
{
	
}

void Toloka::Arrow::Replace(const vec3& v)
{
	center = v;
}

std::vector<float> Toloka::Arrow::GiveData()
{
	std::vector<float> result;
	vec3 ending = center + direction;
	result.push_back(center.x);result.push_back(center.y);result.push_back(center.z);
	result.push_back(color.x);result.push_back(color.y);result.push_back(color.z);
	result.push_back(ending.x);result.push_back(ending.y);result.push_back(ending.z);
	result.push_back(color.x);result.push_back(color.y);result.push_back(color.z);
	return result;
}


Toloka::Toloka(Surface* surface) : active(false), owner(surface) , arrowX(Toloka::Arrow(vec3(0.3f,0,0),vec3(1,0,0))), arrowY(Toloka::Arrow(vec3(0,0.3f,0),vec3(0.4,0.4,0.2))), arrowZ(Toloka::Arrow(vec3(0,0,0.3f),vec3(0,0,1))), arrowActive(NULL)
{
	
}


void Toloka::Update() 
{

	
}


void Toloka::WakeUp(std::vector<Mesh::Point*> ps) 
{


 	vec3 c;
	for(Mesh::Point* p : ps){
		c =  c + p->pos;
	}
	c =  c / ps.size();



	center = c;
	arrowX.Replace(c);
	arrowY.Replace(c);
	arrowZ.Replace(c);
	active = true;
}

void Toloka::Sleep()
{
	
	active = false;
}

std::vector<float> Toloka::GiveData()
{
	

	std::vector<float> result;

	if(!active)return result;


	std::vector<float> r1 = arrowX.GiveData();
	std::vector<float> r2 = arrowY.GiveData();
	std::vector<float> r3 = arrowZ.GiveData();

	result.insert(result.end(),r1.begin(),r1.end());
	result.insert(result.end(),r2.begin(),r2.end());
	result.insert(result.end(),r3.begin(),r3.end());


	

	return result;
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
