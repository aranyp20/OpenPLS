#include <algorithm>

#include "Mesh.h"
#include "Surface.h"
#include "Addons.hpp"
#include "Geometry.h"

void Mesh::PrintVerts() 
{/*
	std::cout << "Matrix is:\n";

	for (int i = 0; i < edgeMatrix.Size(); i++) {
		for (int j = 0; j < edgeMatrix.matrix[i].size(); j++) {
			std::cout << edgeMatrix.matrix[i][j];
		}
		std::cout << "\n";
	}*/
}




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


Mesh::Mesh(Mesh::Shape shape) : corrupted(false)
{


	switch(shape){
		case CUBE: 
			this->TransformToCube();
			break;
	}


	meshRenderer = new MeshRenderer(this);//vegen kell lennie
}


void Mesh::Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	 

	meshRenderer->Render(r, vs, es, ss);


}

void MeshRenderer::Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss)
{
	ss.Bind(); ss.SetUniform("material",*(owner->material));


	sideVAO->Bind();
	RenderData rData = GiveSides();
	sideVBO->RefreshData(rData.raw);	
	
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


	for (Point* c : conns) {
		newColumn[edgeMatrix.elementIndex(c)] = new Edge(c,vert,this);
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

bool Mesh::CheckHit(const vec2& p, const mat4& MVP)
{


	vec2 pp = InputManager::ChangeInput(p);

	std::vector<Hittable*> potHits;
	for(Point* p : points){
		potHits.push_back(p);
	}
	
	EdgeIterator eit = this->begin();
	while(eit.hasNext()){
		potHits.push_back(eit.GetElementEdge());
	}


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
		Corrupt();

		return true;
	}
	return false;
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

int Mesh::EdgeMatrix::elementIndex(Point* p)
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



MeshHandler::MeshHandler(Surface* s) : owner(s)
{
}

InputAnswer MeshHandler::ProcessKey(int key)
{
	if (key == GLFW_KEY_W && CheckHit(InputManager::GetMousePos2())) {
		return InputAnswer(InputAnswer::ReactionType::PROCESSED, NULL);
	}
	else if(key == GLFW_KEY_S){
		return InputAnswer(InputAnswer::ReactionType::BINDED,new OVertScale(activeMesh));
	}
	else if(key == GLFW_KEY_R){
		return InputAnswer(InputAnswer::ReactionType::BINDED,new OVertRotate(activeMesh,vec3(1,0,0)));
	}
	


	return InputAnswer(InputAnswer::ReactionType::IGNORED,NULL);
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

			result.z = pos1.z + (pos2.z - pos1.z) * Interpolate(vec2(pos1.x,pos1.y),vec2(pos2.x,pos2.y),p); //cylinderes megoldas kene mert a projekcio torzizja
		}
	}catch(std::string){
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




