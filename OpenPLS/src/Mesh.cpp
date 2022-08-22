#include <algorithm>

#include "Mesh.h"
#include "Surface.h"
#include "Addons.hpp"
#include "Geometry.h"
#include "Factory.h"


Mesh *MeshHandler::activeMesh = NULL;






void Mesh::TransformToCube()
{
	 std::vector<Mesh::Point*> v;
    std::vector<Mesh::Point*> points;
    points.push_back(new Mesh::Point(-0.5f, -0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, -0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, -0.5f, -0.5f));
    points.push_back(new Mesh::Point(-0.5f, -0.5f, -0.5f));
    points.push_back(new Mesh::Point(-0.5f, 0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, 0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, 0.5f, -0.5f));
    points.push_back(new Mesh::Point(-0.5f, 0.5f, -0.5f));


 
    this->AddPoint(points[0], v);
    v = { points[0] };
    this->AddPoint(points[1], v);
    v.clear();
    v = { points[1] };
    this->AddPoint(points[2], v);
    v.clear();
    v = { points[2],points[0]};
    this->AddPoint(points[3], v);
    v.clear();
    v = { points[0] };
    this->AddPoint(points[4], v);
    v.clear();
    v = { points[1],points[4]};
    this->AddPoint(points[5], v);
    v.clear();
    v = { points[2],points[5] };
   this->AddPoint(points[6], v);
    v.clear();
    v = { points[3],points[6],points[4]};
   this->AddPoint(points[7], v);
    v.clear();


    std::vector<unsigned int> sinds{0, 1, 2, 3};
    this->AddSide(sinds);
    sinds = { 4,5,6,7 };
    this->AddSide(sinds);
    sinds = { 1,2,6,5 };
    this->AddSide(sinds);
    sinds = { 2,6,7,3 };
    this->AddSide(sinds);
    sinds = { 0,3,7,4 };
    this->AddSide(sinds);
	sinds = {0,1,5,4};
	this->AddSide(sinds);

}


Mesh::Mesh(Mesh::Shape shape) : corrupted(false), currentStrat(&stratEDIT)
{


	switch(shape){
		case CUBE: 
			this->TransformToCube();
			break;
	}


	meshRenderer = new MeshRenderer(this);//vegen kell lennie
}

void Mesh::StrategyEditMode::Render(MeshRenderer* mr, const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{

	mr->RenderAsEdit(r, vs, es, ss);
}

void Mesh::Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{

	currentStrat->Render(meshRenderer,r, vs, es, ss);
}

void MeshRenderer::RenderAsEdit(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{

	ss.Bind(); ss.SetUniform("material",*(owner->material));


	sideVAO->Bind();
	RenderData rData = GiveSides();
	sideVBO->RefreshData(rData.raw);	
	
	vertVAO->Bind();
	std::vector<float> empty;
	vertVBO->RefreshData(GiveVertices());
	edgeVAO->Bind();
	edgeVBO->RefreshData(GiveEdges());
	
	

	r.Draw(Renderer::TriangleData(sideVAO,*(rData.ibo),ss),Renderer::PointData(vertVAO,vs),Renderer::LineData(edgeVAO,es));
}

void Mesh::StrategyObjectMode::Render(MeshRenderer* mr,const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	mr->RenderAsObject(r,vs,es,ss);
}
void Mesh::StrategyVertexMode::Render(MeshRenderer* mr,const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	mr->RenderAsVertex(r,vs,es,ss);
}

void MeshRenderer::RenderAsObject(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	ss.Bind(); ss.SetUniform("material",*(owner->material));


	sideVAO->Bind();
	RenderData rData = GiveSides();
	sideVBO->RefreshData(rData.raw);	
	
	vertVAO->Bind();
	std::vector<float> empty;
	vertVBO->RefreshData(empty);
	edgeVAO->Bind();
	edgeVBO->RefreshData(empty);
	
	

	r.Draw(Renderer::TriangleData(sideVAO,*(rData.ibo),ss),Renderer::PointData(vertVAO,vs),Renderer::LineData(edgeVAO,es));
}

void MeshRenderer::RenderAsVertex(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	ss.Bind(); ss.SetUniform("material",*(owner->material));


	std::vector<float> empty;
	sideVAO->Bind();
	RenderData rData = GiveSides();
	sideVBO->RefreshData(empty);	
	
	vertVAO->Bind();
	vertVBO->RefreshData(GiveVertices());
	edgeVAO->Bind();
	edgeVBO->RefreshData(GiveEdges());
	
	

	r.Draw(Renderer::TriangleData(sideVAO,*(rData.ibo),ss),Renderer::PointData(vertVAO,vs),Renderer::LineData(edgeVAO,es));
}

MeshRenderer::MeshRenderer(Mesh* _owner) : owner(_owner)
{
	vertVAO = new VAO();
	edgeVAO = new VAO();
	sideVAO = new VAO();
	vertVBO = new VBO3f3f(GiveVertices());
	edgeVBO = new VBO3f3f(GiveEdges());
	sideVBO = new VBO3f3f(GiveSides().raw);


	vertVAO->AddVBO(*vertVBO);

	edgeVAO->AddVBO(*edgeVBO);

	sideVAO->AddVBO(*sideVBO);
}

void MeshHandler::Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	activeMesh->Render(r,vs,es,ss);
}

void Mesh::AddPoint(Point* vert, const std::vector<Point*> conns)
{

	vert->owner=this;

	std::vector<Edge*> newColumn(edgeMatrix.Size() + 1);

	for(auto& e : newColumn){
		e = NULL;
	}


	for (Point* c : conns) {
		newColumn[edgeMatrix.elementIndex(c)] = new Edge(c,vert,this);
	}


		
	points.push_back(vert);
	edgeMatrix.orderVec.push_back(vert);

	edgeMatrix.matrix.push_back(newColumn);
}

void Mesh::AddPoint(Point *vert)
{
	vert->owner=this;

	std::vector<Edge*> newColumn(edgeMatrix.Size() + 1);

	for(auto& e : newColumn){
		e = NULL;
	}

		
	points.push_back(vert);
	edgeMatrix.orderVec.push_back(vert);

	edgeMatrix.matrix.push_back(newColumn);
}

void Mesh::AddSide(std::vector<unsigned int>& is)
{
	std::vector<Point*> res;
	for (int i : is) {
		if (i >= 0 && i < points.size()) {
			res.push_back(points[i]);

			
		}
	}
	sides.push_back(new Side(res));
}

bool Mesh::Strategy::CheckHitCommon(Mesh* _m,const vec2& p, const mat4& MVP,std::vector<Hittable*>& potHits)
{
	vec2 pp = p;
	float closestZ = 2.0f;
	Hittable* closestHit = NULL;
	for(Hittable* h : potHits){
		Hittable::Hit curHit = h->Intersect(pp,MVP);
		if(curHit.z<1&&curHit.z>-1&&curHit.z<closestZ){
			closestHit=curHit.hittable;
			closestZ = curHit.z;
		}
	}
	
	if (closestHit != NULL) {
		closestHit->NotifyWin();
		_m->Corrupt();

		return true;
	}
	return false;
}


bool Mesh::StrategyObjectMode::CheckHit(Mesh* _m,const vec2& p, const mat4& MVP)
{
	std::vector<Hittable*> potHits;
	for(Point* p : _m->points){
		potHits.push_back(p);
	}
	
	EdgeIterator eit = _m->begin();
	while(eit.hasNext()){
		potHits.push_back(eit.GetElementEdge());
	}

	return CheckHitCommon(_m,p,MVP,potHits);
}

bool Mesh::StrategyEditMode::CheckHit(Mesh* _m,const vec2& p, const mat4& MVP)
{
	std::vector<Hittable*> potHits;
	for(Point* p : _m->points){
		potHits.push_back(p);
	}
	EdgeIterator eit = _m->begin();
	while(eit.hasNext()){
		potHits.push_back(eit.GetElementEdge());
	}

	return CheckHitCommon(_m,p,MVP,potHits);
}
bool Mesh::StrategyVertexMode::CheckHit(Mesh* _m,const vec2& p, const mat4& MVP)
{
	std::vector<Hittable*> potHits;
	for(Point* p : _m->points){
		potHits.push_back(p);
	}
	EdgeIterator eit = _m->begin();
	while(eit.hasNext()){
		potHits.push_back(eit.GetElementEdge());
	}

	return CheckHitCommon(_m,p,MVP,potHits);
}

bool Mesh::CheckHit(const vec2& p, const mat4& MVP)
{
	return currentStrat->CheckHit(this,p,MVP);
}

//fel-le gyorsabban megy mint oldalra szoval aspecttel osztani kene
void OVertMove::Update()
{
	FindMidPoint();
	mat4 VP = camera->V()*camera->P();
	vec3 wEye = camera->GetEye();

	vec3 centerCam = midPoint; TransformPoint(centerCam,VP); centerCam = camera->PutToWorld(vec2(centerCam.x,centerCam.y));
	vec3 endWorld = midPoint + axis;
	vec3 endCam = endWorld; TransformPoint(endCam,VP);endCam = camera->PutToWorld(vec2(endCam.x,endCam.y));
	vec3 mouseCamLast = camera->PutToWorld(InputManager::ChangeInput(InputManager::GetMousePos1()));
	vec3 mouseCam = camera->PutToWorld(InputManager::ChangeInput(InputManager::GetMousePos2()));
	vec3 projTemp = normalize(centerCam-endCam); 
	vec3 moveCam = projTemp * dot(vec3(mouseCam-mouseCamLast),projTemp);
	vec3 moveWorld = moveCam *  ((wEye-endWorld).length() / (wEye-endCam).length());
	


	vec3 moveVector = normalize(axis) * dot(moveWorld,normalize(axis));


	for(auto p : controlledPoints){ //ez nem jo hogy belenyul
		p->pos = p->pos + moveVector;

	}
	FindMidPoint();
}

bool Mesh::ReleaseSelection()
{
	if (!selectedPoints.empty()) {
		selectedPoints.clear();
		Corrupt();
		return true;
	}
	return false;
}

void Mesh::Transform(const mat4& m)
{
	M = M * m;
}

std::vector<Mesh::Point*> Mesh::GiveSelecteds()
{
	return selectedPoints;
}

Mesh::EdgeIterator::EdgeIterator(Mesh& _parent, int _row = 0, int _column = 0):parent(_parent),row(_row),column(_column)
{

}

Mesh::EdgeIterator Mesh::begin()
{
	return EdgeIterator(*this);
}

//nem valoszinu hogy jo
//Mesh::EdgeIterator Mesh::EdgeIterator::end()
//{
//	return EdgeIterator(parent, parent.edgeMatrix.Size()-1,parent.edgeMatrix.Size()-2);
//}

Mesh::Point* Mesh::EdgeIterator::GetElement1()
{
	return parent.edgeMatrix.indexElement(row);
}

Mesh::Point* Mesh::EdgeIterator::GetElement2()
{
	return parent.edgeMatrix.indexElement(column);
}


Mesh::Edge* Mesh::EdgeIterator::GetElementEdge() 
{
	return parent.edgeMatrix.matrix[column][row];
}

bool Mesh::EdgeIterator::hasNext()
{
	

	do {
		int highestValidRow = column;
		if (row == highestValidRow) {
			if (column < parent.edgeMatrix.Size() - 1) {
				row = 0;
				column++;
			}
			else {
				return false;
			}
		}
		else {
			row++;
		}
	} while (parent.edgeMatrix.matrix[column][row] == NULL);
	
	return true;
}

//Mesh::EdgeIterator& Mesh::EdgeIterator::operator++()
//{
//	int highestValidRow = column - 1;
//	if (row == highestValidRow) {
//		if (column < parent.edgeMatrix.size() - 1) {
//			row = 0;
//			column++;
//		}
//		else {
//			//end
//		}
//	}
//	else {
//		row++;
//	}
//
//	return *this;
//}

unsigned int Mesh::EdgeMatrix::elementIndex(Point* p)
{
	std::vector<Point*>::iterator tempIterator = std::find(orderVec.begin(), orderVec.end(), p);

	if (tempIterator == orderVec.end())return -1;

	return tempIterator - orderVec.begin();
}

Mesh::Point* Mesh::EdgeMatrix::indexElement(int i)
{
	return orderVec[i];
}


void Mesh::SelectPoint(Point* p) 
{
	if(VectorContains<Point>(selectedPoints,p))return;
	selectedPoints.push_back(p);
}

//MEMORY LEAK
RenderData MeshRenderer::GiveSides()
{
	RenderData res;
	
	std::vector<float> f;
	std::vector<unsigned int> inds;
	


	unsigned int iHelp = 0;

	for (Mesh::Side *s : owner->sides) {
		if (s->points.size() == 4) {
			vec3 norm1 = cross(s->points[0]->pos - s->points[1]->pos, s->points[0]->pos - s->points[2]->pos);
			vec3 norm2 = cross(s->points[3]->pos - s->points[2]->pos, s->points[3]->pos - s->points[0]->pos);

			for (int i = 0; i < 3; i++) {
				f.push_back(s->points[i]->pos.x);
				f.push_back(s->points[i]->pos.y);
				f.push_back(s->points[i]->pos.z);
				f.push_back(norm1.x);
				f.push_back(norm1.y);
				f.push_back(norm1.z);

			}
			for (int i = 0; i < 4; i++) {
				if(i!=1){
					f.push_back(s->points[i]->pos.x);
					f.push_back(s->points[i]->pos.y);
					f.push_back(s->points[i]->pos.z);
					f.push_back(norm2.x);
					f.push_back(norm2.y);
					f.push_back(norm2.z);
				}
				

			}



/* 			for (int i = 0; i < 4; i++) {
				f.push_back(s->points[i]->pos.x);
				f.push_back(s->points[i]->pos.y);
				f.push_back(s->points[i]->pos.z);
				f.push_back(norm.x);
				f.push_back(norm.y);
				f.push_back(norm.z);

			}


			inds.push_back(iHelp * 4);
			inds.push_back(iHelp * 4 + 1);
			inds.push_back(iHelp * 4 + 2);

			inds.push_back(iHelp * 4);
			inds.push_back(iHelp * 4 + 2);
			inds.push_back(iHelp * 4 + 3); 

			iHelp++; */
		}
	}
	res.raw = f;
	if(res.ibo!=NULL)delete res.ibo;
	if(res.vbo!=NULL)delete res.vbo;
	res.ibo = new IBO(inds);
	res.vbo = new VBO3f3f(f);

	return res;
}

std::vector<float> MeshRenderer::GiveVertices()
{
	std::vector<float> res;
	for (Mesh::Point* p : owner->points) {
		res.push_back(p->pos.x);
		res.push_back(p->pos.y);
		res.push_back(p->pos.z);
		if (VectorContains<Mesh::Point>(owner->selectedPoints, p)) {
			res.push_back(1); res.push_back(1); res.push_back(1);
		}
		else {
			res.push_back(0); res.push_back(0); res.push_back(0);
		}
		
	}

	return res;
}

std::vector<float> MeshRenderer::GiveEdges()
{
	
	std::vector<float> res;
	Mesh::EdgeIterator it = owner->begin();
	while (it.hasNext()) {

		res.push_back(it.GetElement1()->pos.x);
		res.push_back(it.GetElement1()->pos.y);
		res.push_back(it.GetElement1()->pos.z);
		if(VectorContains<Mesh::Point>(owner->selectedPoints,it.GetElement1())){
			res.push_back(1); res.push_back(1); res.push_back(1);
		}else{
			res.push_back(0); res.push_back(0); res.push_back(0);
		}
		
		res.push_back(it.GetElement2()->pos.x);
		res.push_back(it.GetElement2()->pos.y);
		res.push_back(it.GetElement2()->pos.z);
		if(VectorContains<Mesh::Point>(owner->selectedPoints,it.GetElement2())){
			res.push_back(1); res.push_back(1); res.push_back(1);
		}else{
			res.push_back(0); res.push_back(0); res.push_back(0);
		}
	}
	return res;
}



void Mesh::ChangeMode(const Mesh::Mode& _m)
{
	switch(_m){
		case Mode::OBJECT:
			currentStrat = &stratOBJ;
		break;
		case Mode::EDIT:
			currentStrat = &stratEDIT;
		break;
		case Mode::VERTEX:
			currentStrat = &stratVERTEX;
		break;
	}
}



MeshHandler::MeshHandler(Surface* s) : owner(s)
{
}

InputAnswer MeshHandler::ProcessKey(int key)
{
	if (key == GLFW_KEY_W && CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2()))) {
		return InputAnswer(InputAnswer::ReactionType::PROCESSED);
	}
	else if(key == GLFW_KEY_S){
		return InputAnswer(InputAnswer::ReactionType::CREATE,InputAnswer::OperationType::VERT_SCALE);
	}
	else if(key == GLFW_KEY_R){
		return InputAnswer(InputAnswer::ReactionType::CREATE,InputAnswer::OperationType::VERT_ROTATE);
	}else if(key == GLFW_KEY_E){
		OVertExtrude ov(activeMesh);
		return InputAnswer(InputAnswer::ReactionType::PROCESSED);
	}
	


	return InputAnswer();
}

InputAnswer MeshHandler::ProcessMouseClick()
{
	if(InputManager::GetFactory()->TriggerHolded() != NULL)return InputAnswer(InputAnswer::ReactionType::PROCESSED);


	if (CheckHit(InputManager::ChangeInput(InputManager::GetMousePos2()))) {
		return InputAnswer(InputAnswer::ReactionType::PROCESSED);
	}
	return InputAnswer();
}

void MeshHandler::AddMesh(Mesh* m)
{
	activeMesh = m;
	meshes.push_back(m);
}

bool MeshHandler::CheckHit(const vec2& p)
{
	
	if (activeMesh->CheckHit(p, owner->viewCamera->V() * owner->viewCamera->P())) {
		owner->toloka->WakeUp(activeMesh->GiveSelecteds());//nyilvan szebben lesz
		return true; 
	}
	if(activeMesh->ReleaseSelection()){owner->toloka->Sleep();return true;} 
	return false;
}



vec3 OVertMove::GetMidPoint(){return midPoint;}

OVertMove::OVertMove(Mesh* _mesh,vec3 _axis,Camera* _camera) : MeshOperation(_mesh) , axis(_axis) ,  camera(_camera) 
{	
}



Hittable::Hit Mesh::Point::Intersect(const vec2& pp, const mat4& MVP) 
{
	
	Hittable::Hit result;
	vec3 vpos = pos;
	TransformPoint(vpos, MVP);
	if((pp-vec2(vpos.x,vpos.y)).length()<0.015f){
		result.z = vpos.z - 0.01f;
		result.hittable = this;
	}
	return result;
}

void Mesh::Point::NotifyWin() 
{
	owner->SelectPoint(this);
}

Hittable::Hit Mesh::Edge::Intersect(const vec2& p, const mat4& MVP) 
{
	Hittable::Hit result;
	vec3 pos1 = p1->pos;
	vec3 pos2 = p2->pos;
	TransformPoint(pos1,MVP);
	TransformPoint(pos2,MVP);
	try{
		float dist = Line2D(vec2(pos1.x,pos1.y),vec2(pos2.x,pos2.y)).DistanceFromSection(p,false);
		
		if(dist < 0.015f && dist>-0.5f){
		
			result.hittable=this;

			result.z = pos1.z + (pos2.z - pos1.z) * Interpolate(vec2(pos1.x,pos1.y),vec2(pos2.x,pos2.y),p); 
		}
	}catch(GeometryException){
		return result;
	}
	
	return result;
	
}


void Mesh::Edge::NotifyWin() 
{
	owner->SelectPoint(p1);
	owner->SelectPoint(p2);
}

void MeshOperation::FindMidPoint()
{
	vec3 newMidPoint;
	for(Mesh::Point* p : controlledPoints){
		newMidPoint = newMidPoint + p->pos;
	}
	midPoint = newMidPoint / controlledPoints.size();
}

MeshOperation::MeshOperation(Mesh* m) : controlledPoints(m->GiveSelecteds()), startingPos(InputManager::ChangeInput(InputManager::GetMousePos2()))
{
	FindMidPoint();

}

OVertScale::OVertScale(Mesh* m) : MeshOperation(m)
{



	for(Mesh::Point* p : controlledPoints){
		originalDefficit.push_back((p->pos-midPoint));
	}
}

void OVertScale::Update()
{
	float multiplier = dot(InputManager::ChangeInput(InputManager::GetMousePos2())-startingPos,vec2(1,0));

	for(int i = 0; i< controlledPoints.size();i++){
		controlledPoints[i]->pos = midPoint + originalDefficit[i] * (multiplier + 1);
	}
}


OVertRotate::OVertRotate(Mesh* m, vec3 _axis) : MeshOperation(m), axis(_axis)
{

}

void OVertRotate::Update()
{
	float multiplier = dot((InputManager::ChangeInput(InputManager::GetMousePos2()) - InputManager::ChangeInput(InputManager::GetMousePos1())),vec2(1,0));
	mat4 transformator = RotationMatrix(multiplier,axis);
	for(Mesh::Point* p : controlledPoints){
		p->pos = p->pos -  midPoint;
		TransformPoint(p->pos,transformator);
		p->pos = p->pos + midPoint;
	}
}




OVertExtrude::OVertExtrude(Mesh* mesh) : MeshOperation(mesh)
{
	std::vector<Mesh::Side*> selectedSides;
	std::vector<Mesh::Point*> validSelection;
	for(auto& a : mesh->sides){
		bool someThingMissing = false;
		for(auto& b : a->points){
		
			if(!(std::find(controlledPoints.begin(),controlledPoints.end(),b)!=controlledPoints.end())){
				someThingMissing = true;
			}
		}
		if(!someThingMissing){
			selectedSides.push_back(a);
			for(auto& b : a->points){

				if(!(std::find(validSelection.begin(),validSelection.end(),b)!=validSelection.end())){
					validSelection.push_back(b);
				}
			}
		}
	}


	std::map<Mesh::Side*,std::vector<Mesh::Point*>> hozza;

	std::vector<Mesh::Point*> newPoints;
	
	for(auto p : validSelection){
		Mesh::Point* newPoint = new Mesh::Point(*p);
		newPoints.push_back(newPoint);
		mesh->AddPoint(newPoint);

		for(auto& a : selectedSides){
			if(std::find(a->points.begin(),a->points.end(),p)!=a->points.end()){
				hozza[a].push_back(newPoint);
			}
		}
	}


	
	

	std::vector<std::vector<Mesh::Point*>> emptyFrames;

	for(int i=0;i<newPoints.size();i++){
		for(int j=i+1;j<newPoints.size();j++){
			if(mesh->edgeMatrix.GetRelationshipBetweenPoints(validSelection[i],validSelection[j]) != NULL){
				
				mesh->edgeMatrix.SetRelationshipBetweenPoints(newPoints[i],newPoints[j],new Mesh::Edge(newPoints[i],newPoints[j],mesh));
				
				std::vector<Mesh::Point*> tempP;tempP.push_back(newPoints[i]);tempP.push_back(newPoints[j]);tempP.push_back(validSelection[i]);tempP.push_back(validSelection[j]);
				
				
				emptyFrames.push_back({newPoints[i],newPoints[j],validSelection[i],validSelection[j]});
			}
		}
	}

	for(int i = 0;i<newPoints.size();i++){
		mesh->edgeMatrix.SetRelationshipBetweenPoints(newPoints[i],validSelection[i],new Mesh::Edge(newPoints[i],validSelection[i],mesh));
	}

	for(auto& a : emptyFrames){
		
		mesh->AddSideExtra(a);
	}

	for(auto& a : selectedSides){
		
		mesh->AddSideExtra(hozza[a]);

		mesh->DeleteSide(a);
		delete a;
	}

	mesh->selectedPoints = newPoints;
}

void Mesh::AddSideExtra(std::vector<Point*> p)
{
	std::vector<Point*> ordered;
	std::vector<Point*> remaining;

	std::copy(p.begin(), p.end(), back_inserter(remaining));

	ordered.push_back(remaining[0]);
	remaining.erase(remaining.begin());

	for(int i=0;i<p.size();i++){

		EdgeIterator it = begin();
		bool found = false;

		while(!found && it.hasNext()){
			


			if(it.GetElement1() == ordered[i] && std::find(remaining.begin(),remaining.end(),it.GetElement2())!=remaining.end()){	
				remaining.erase(std::remove(remaining.begin(), remaining.end(), it.GetElement2()), remaining.end());
				ordered.push_back(it.GetElement2());
				
				
			
				found=true;
				
			}else if(it.GetElement2() == ordered[i] && std::find(remaining.begin(),remaining.end(),it.GetElement1())!=remaining.end()){		
				remaining.erase(std::remove(remaining.begin(), remaining.end(), it.GetElement1()), remaining.end());
				ordered.push_back(it.GetElement1());
				found=true;
			}
		}
	}

	std::vector<unsigned int> result;

	for(auto& l : ordered){
		result.push_back(edgeMatrix.elementIndex(l));
	}
	



	AddSide(result);
}

void OVertExtrude::Update()
{

}

Mesh::Edge* Mesh::EdgeMatrix::GetRelationshipBetweenPoints(Point* p1,Point* p2)
{
	int placeP1 = elementIndex(p1);
	int placeP2 = elementIndex(p2);


	if(placeP1<matrix[placeP2].size() && matrix[placeP2][placeP1] != NULL){
		return matrix[placeP2][placeP1];
	}else if(placeP2<matrix[placeP1].size() && matrix[placeP1][placeP2] != NULL){
		return matrix[placeP1][placeP2];
	}

	return NULL;

}

void Mesh::EdgeMatrix::SetRelationshipBetweenPoints(Point* p1, Point* p2, Edge* edge)
{
	int placeP1 = elementIndex(p1);
	int placeP2 = elementIndex(p2);


	if(placeP1<matrix[placeP2].size() ){
		matrix[placeP2][placeP1] = edge;
	}else if(placeP2<matrix[placeP1].size()){
		matrix[placeP1][placeP2] = edge;
	}

}


void Mesh::DeleteEdge(Edge* _e)
{
 	EdgeIterator it = begin();
	std::vector<std::vector<unsigned int>> deleteThese;
	while(it.hasNext()){
		if(it.GetElementEdge()==_e){
			std::vector<unsigned int> ta {edgeMatrix.elementIndex(it.GetElement1()),edgeMatrix.elementIndex(it.GetElement2())} ;
			deleteThese.push_back(ta);
		}
	}
	for(auto& a : deleteThese){
		
		edgeMatrix.matrix[a[1]][a[0]] = NULL;
	} 
}

void Mesh::AddSide(std::vector<Mesh::Point*>& ps)
{
	sides.push_back(new Side(ps));
}

void Mesh::DeleteSide(Side* _s)
{
	sides.erase(std::remove(sides.begin(),sides.end(),_s),sides.end());
}


vec3 Mesh::CalculateMidPoint(std::vector<Point*> ps)
{
	vec3 sum;
	for(auto& c : ps){
		sum = sum + c->pos;
	}
	sum = sum / ps.size();
	return sum;
}

std::vector<Mesh::Edge*> Mesh::Side::GetEdges(Mesh* m)
{
	std::vector<Mesh::Edge*> result;
	for(int i = 0;i<points.size();i++){
		int i1 = i;
		int i2 = i+1;
		if(i2==points.size())i2 = 0;
		Mesh::EdgeIterator it = m->begin();
		while(it.hasNext()){
			if((it.GetElement1()==points[i1] && it.GetElement2()==points[i2]) || (it.GetElement1()==points[i2] && it.GetElement2()==points[i1]))result.push_back(it.GetElementEdge());
		}
	}
	return result;
}



//////////////////////////////////////////////////////////////////////////////////////////
void OVertSubdivide::InfluenceMap::AddInfluence(Mesh::Point* for_what,Mesh::Point* what)
{
	if(infMap.count(for_what)==1){
		if(!(std::find(infMap[for_what].begin(),infMap[for_what].end(),what)!=infMap[for_what].end())){
			infMap[for_what].push_back(what);
		}		
		return;
	}
	infMap[for_what] = std::vector<Mesh::Point*>{what};
}


OVertSubdivide::OVertSubdivide(Mesh* _mesh) : MeshOperation(_mesh), myMesh(_mesh)
{
	//std::cout<<"1"<<std::endl;
	SnapShot();
	//std::cout<<"2"<<std::endl;
	FillEdgePoints();
	//std::cout<<"3"<<std::endl;
	FillSidePoints();
	//std::cout<<"4"<<std::endl;
	FillOldPoints();
	//std::cout<<"5"<<std::endl;
	ReplaceEdgePoints();
	//std::cout<<"6"<<std::endl;
	ReplaceOldPoints();
	//std::cout<<"7"<<std::endl;
	CreateNewMesh();
	//std::cout<<"8"<<std::endl;
	DeleteOldSidesAndEdges();
	//std::cout<<"done"<<std::endl;
}

OVertSubdivide::~OVertSubdivide()
{
	for(auto& a : edgePoints)delete a;
	for(auto& a : sidePoints)delete a;
	
}

void OVertSubdivide::SnapShot()
{
	Mesh::EdgeIterator it = myMesh->begin();
	while(it.hasNext()){
		OriginalEdges.push_back(it.GetElementEdge());
	}
	for(auto& a : myMesh->sides){
		OriginalSides.push_back(a);
	}
}

void OVertSubdivide::FillEdgePoints()
{
	Mesh::EdgeIterator it = myMesh->begin();
	while(it.hasNext()){
		EdgePoint* newEP = new EdgePoint(it.GetElementEdge());
		edgePoints.push_back(newEP);

		im.AddInfluence(it.GetElement1(),newEP->me);
		im.AddInfluence(it.GetElement2(),newEP->me);
	}
}

void OVertSubdivide::FillSidePoints()
{
	for(auto& s : myMesh->sides){
		std::vector<Mesh::Edge*> es = s->GetEdges(myMesh);
		std::vector<OVertSubdivide::EdgePoint*> eps;
		for(auto& e : es){
			for(auto& ep : edgePoints){
				if(ep->myEdge == e){
					eps.push_back(ep);
				}
			}
		}
		SidePoint* newSP = new SidePoint(s,eps);
		sidePoints.push_back(newSP);

		for(auto& a : s->points){
			im.AddInfluence(a,newSP->me);
		}
	}
}

void OVertSubdivide::FillOldPoints()
{
	oldPoints = myMesh->points;
}
void OVertSubdivide::ReplaceEdgePoints()
{
	for(auto& a : edgePoints){
		std::vector<Mesh::Point*> influencerList;
		influencerList.push_back(a->myEdge->p1); 
		influencerList.push_back(a->myEdge->p2); 
		for(auto& b : sidePoints){
			if(std::find(b->ePoints.begin(),b->ePoints.end(),a)!=b->ePoints.end()){
				influencerList.push_back(b->me);
				if(influencerList.size()==4)break;
			}
		}
		a->me->pos = Mesh::CalculateMidPoint(influencerList);
	}



}

void OVertSubdivide::ReplaceOldPoints()
{
	for(auto& a : oldPoints){
		vec3 temp1 = Mesh::CalculateMidPoint(im.infMap[a]);
		a->pos = (a->pos + temp1) / 2;
	}
}

void OVertSubdivide::CreateNewMesh()
{
	for(auto& te : edgePoints){
		std::vector<Mesh::Point*> tConns {te->myEdge->p1,te->myEdge->p2};
		myMesh->AddPoint(te->me,tConns);
	}
	int i = 0;
	for(auto& te : sidePoints){
		
		std::vector<Mesh::Point*> tConns;
		for(auto& tt : te->ePoints)tConns.push_back(tt->me);
		myMesh->AddPoint(te->me,tConns);
		
		for(unsigned int i = 0;i<te->ePoints.size();i++){
			
			unsigned int i1=i;
			unsigned int i2 =i+1;
			if(i2==te->ePoints.size())i2=0;
			Mesh::Point* commonP = NULL;
			if(te->ePoints[i1]->myEdge->p1 == te->ePoints[i2]->myEdge->p1) commonP = te->ePoints[i1]->myEdge->p1;
			else if(te->ePoints[i1]->myEdge->p1 == te->ePoints[i2]->myEdge->p2) commonP = te->ePoints[i1]->myEdge->p1;
			else if(te->ePoints[i1]->myEdge->p2 == te->ePoints[i2]->myEdge->p1) commonP = te->ePoints[i1]->myEdge->p2;
			else if(te->ePoints[i1]->myEdge->p2 == te->ePoints[i2]->myEdge->p2) commonP = te->ePoints[i1]->myEdge->p2;
			std::vector<Mesh::Point*> sss {commonP, te->ePoints[i1]->me,te->me,te->ePoints[i2]->me};
			
			myMesh->AddSide(sss);
			
		}

	}	
}

void OVertSubdivide::DeleteOldSidesAndEdges()
{
	for(auto& a : OriginalEdges){
		myMesh->DeleteEdge(a); delete a;
	}
	for(auto& a : OriginalSides){
		myMesh->DeleteSide(a); delete a;
	}
}

OVertSubdivide::EdgePoint::EdgePoint(Mesh::Edge* _edge) : myEdge(_edge)
{
	me = new Mesh::Point((_edge->p2->pos+_edge->p1->pos)/2);
}

OVertSubdivide::SidePoint::SidePoint(Mesh::Side* _side, std::vector<OVertSubdivide::EdgePoint*> ep) : ePoints(ep)
{
	me = new Mesh::Point(Mesh::CalculateMidPoint(_side->points));
	
}



