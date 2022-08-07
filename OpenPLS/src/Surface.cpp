#include "Surface.h"
#include "Geometry.h"

InputAnswer Surface::ProcessKey(int key)
{
	if (key == GLFW_KEY_Q) {
		return InputAnswer(InputAnswer::ReactionType::BINDED, new OCameraMove(viewCamera));
	}else if(key == GLFW_KEY_A){
		if(toloka->CheckHit()){
			return InputAnswer(InputAnswer::ReactionType::BINDED, toloka);
		}
	}

	return InputAnswer(InputAnswer::ReactionType::IGNORED, NULL);
	
}

bool Toloka::CheckHit()
{
	if(!active)return false;
	if(arrowX.CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2()),owner->viewCamera->V()*owner->viewCamera->P())){
		arrowActive = &(this->arrowX);

		return true;
	}else if(arrowY.CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2()),owner->viewCamera->V()*owner->viewCamera->P())){
		arrowActive = &(this->arrowY);

		return true;
	}else if(arrowZ.CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2()),owner->viewCamera->V()*owner->viewCamera->P())){
		arrowActive = &(this->arrowZ);

		return true;
	}
	return false;
}

bool Toloka::Arrow::CheckHit(const vec2& p, const mat4& m)
{
	vec3 tp3 = vec3(center+direction); TransformPoint(tp3,m);
	vec2 tp2 = vec2(tp3.x,tp3.y);
	return Circle(tp2,0.035f).Contains(p);
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


Toloka::Toloka(Surface* surface) : active(false), owner(surface) , arrowX(Toloka::Arrow(vec3(0.2f,0,0),vec3(1,0,0))), arrowY(Toloka::Arrow(vec3(0,0.2f,0),vec3(0,1,0))), arrowZ(Toloka::Arrow(vec3(0,0,0.2f),vec3(0,0,1))), arrowActive(NULL)
{
	
}

void Toloka::Replace(const vec3& v)
{
	
	center = v;
	arrowX.Replace(v);
	arrowY.Replace(v);
	arrowZ.Replace(v);
}


//valami nem jo vele
//ami biztos hiba az a pontatlansag hogy mindig adogatok egy vektort, de azon kivul is lehet nem tul jo
void Toloka::Update() 
{
	mat4 VP = owner->viewCamera->V()*owner->viewCamera->P();
	vec3 wEye = owner->viewCamera->GetEye();

	vec3 centerCam = center; TransformPoint(centerCam,VP); centerCam = owner->viewCamera->PutToWorld(vec2(centerCam.x,centerCam.y));
	vec3 endWorld = center + arrowActive->direction;
	vec3 endCam = endWorld; TransformPoint(endCam,VP);endCam = owner->viewCamera->PutToWorld(vec2(endCam.x,endCam.y));
	vec3 mouseCamLast = owner->viewCamera->PutToWorld(InputManager::ChangeInput(InputManager::GetMousePos1()));
	vec3 mouseCam = owner->viewCamera->PutToWorld(InputManager::ChangeInput(InputManager::GetMousePos2()));
	vec3 projTemp = normalize(centerCam-endCam); 
	vec3 moveCam = projTemp * dot(vec3(mouseCam-mouseCamLast),projTemp);
	vec3 moveWorld = moveCam *  ((wEye-endWorld).length() / (wEye-endCam).length());
	


	vec3 moveVector = normalize(arrowActive->direction) * dot(moveWorld,normalize(arrowActive->direction));


	for(Mesh::Point* p : controlledPoints){ //ez nem jo hogy belenyul
		p->pos = p->pos + moveVector;

	}
	

	Replace(center + moveVector);
}


void Toloka::WakeUp(std::vector<Mesh::Point*> ps) 
{


 	vec3 c;
	for(Mesh::Point* p : ps){
		c =  c + p->pos;
	}
	c =  c / ps.size();

	controlledPoints = ps;


	Replace(c);
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
