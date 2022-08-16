#include "Surface.h"
#include "Geometry.h"
#include "Factory.h"


InputAnswer Surface::ProcessMouseClick()
{
	if(InputManager::GetFactory()->TriggerHolded() != NULL)return InputAnswer(InputAnswer::ReactionType::PROCESSED);
	if(toloka->CheckHit()){
			InputManager::ChangeBind(toloka);
			return InputAnswer(InputAnswer::ReactionType::PROCESSED);
	}
	return InputAnswer();
}

InputAnswer Surface::ProcessKey(int key)
{
	if (key == GLFW_KEY_Q) {
		return InputAnswer(InputAnswer::ReactionType::CREATE, InputAnswer::OperationType::CAMERA_MOVE);
	}else if(key == GLFW_KEY_A){
		if(toloka->CheckHit()){
			InputManager::ChangeBind(toloka);
			return InputAnswer(InputAnswer::ReactionType::PROCESSED);
		}
	}
	
	return InputAnswer();
	
}

bool Toloka::CheckHit()
{
	if(!active)return false;

	for(auto arrow : arrows){
		if(arrow.CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2()),owner->viewCamera->V()*owner->viewCamera->P())){
			arrowActive = arrow;
			moveOperation = static_cast<OVertMove*>(InputManager::GetFactory()->CreateOperation(InputAnswer::OperationType::VERT_MOVE,Factory::CreationAddons(arrowActive.direction)));
			return true;
		}
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

void Surface::Render(Renderer& renderer)
{


	mat4 mod;
	shader.PrepareForRendering(GouraudShader::Data(light,viewCamera->GetEye(),mod,viewCamera->V(),viewCamera->P(),mod));
    shader1.PrepareForRendering(NormalShader::Data(mod,viewCamera->V(),viewCamera->P()));
  
    meshHandler->Render(renderer,shader1,shader1,shader);

	glClear(GL_DEPTH_BUFFER_BIT);

    topLayerVAO->Bind();
    topLayerVBO->RefreshData(toloka->GiveData());
    renderer.DrawL(*topLayerVAO,shader1);

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


Toloka::Toloka(Surface* surface) : active(false), owner(surface) 
{
	arrows.push_back(Toloka::Arrow(vec3(0.2f,0,0),vec3(1,0,0)));
	arrows.push_back(Toloka::Arrow(vec3(0,0.2f,0),vec3(0,1,0)));
	arrows.push_back(Toloka::Arrow(vec3(0,0,0.2f),vec3(0,0,1)));
}

void Toloka::Replace(const vec3& v)
{
	
	center = v;
	for (auto& arrow : arrows){
		arrow.Replace(v);
	}

}



void Toloka::Update() 
{
	moveOperation->Update();
	Replace(moveOperation->GetMidPoint());
}





void Toloka::WakeUp(std::vector<Mesh::Point*> ps) 
{


 	vec3 c;
	for(auto p : ps){
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



	for(auto arrow: arrows){
		std::vector<float> r = arrow.GiveData();
		result.insert(result.end(),r.begin(),r.end());
	} 
	


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

Surface::Surface() : toloka(new Toloka(this))
{
	topLayerVAO = new VAO();
	topLayerVBO = new VBO3f3f(toloka->GiveData());
	topLayerVAO->AddVBO(*topLayerVBO);
}
