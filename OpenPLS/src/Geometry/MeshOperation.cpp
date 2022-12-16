#include <algorithm>
#include "MeshOperation.h"


//ennek a baja hogy a surface atallitja a viewportot (90%)
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

vec3 OVertMove::GetMidPoint(){return midPoint;}

OVertMove::OVertMove(Mesh* _mesh,vec3 _axis,Camera* _camera) : MeshOperation(_mesh) , axis(_axis) ,  camera(_camera) 
{	
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
		Mesh::Point* newPoint = Mesh::pBlock.Allocate(*p);
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
				
				mesh->edgeMatrix.SetRelationshipBetweenPoints(newPoints[i],newPoints[j],Mesh::eBlock.Allocate(Mesh::Edge(newPoints[i],newPoints[j],mesh)));
				
				std::vector<Mesh::Point*> tempP;tempP.push_back(newPoints[i]);tempP.push_back(newPoints[j]);tempP.push_back(validSelection[i]);tempP.push_back(validSelection[j]);
				
				
				emptyFrames.push_back({newPoints[i],newPoints[j],validSelection[i],validSelection[j]});
			}
		}
	}

	for(int i = 0;i<newPoints.size();i++){
		mesh->edgeMatrix.SetRelationshipBetweenPoints(newPoints[i],validSelection[i],Mesh::eBlock.Allocate(Mesh::Edge(newPoints[i],validSelection[i],mesh)));
	}

	for(auto& a : emptyFrames){
		
		mesh->AddSideExtra(a);
	}

	for(auto& a : selectedSides){
		
		mesh->AddSideExtra(hozza[a]);

		mesh->DeleteSide(a);
		Mesh::sBlock.Delete(a);
	}

	mesh->selectedPoints = newPoints;
}

void OVertExtrude::Update()
{

}
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
		edgePoints.push_back(EdgePoint(it.GetElementEdge()));

		im.AddInfluence(it.GetElement1(),newEP->me);
		im.AddInfluence(it.GetElement2(),newEP->me);
	}
}

void OVertSubdivide::FillSidePoints()
{
	for(auto& s : myMesh->sides){
		std::vector<Mesh::Edge*> es = s->GetEdges(myMesh);
		std::vector<OVertSubdivide::EdgePoint> eps;
		for(auto& e : es){
			for(auto& ep : edgePoints){
				if(ep.myEdge == e){
					eps.push_back(ep);
				}
			}
		}
		SidePoint newSp(s,eps);
		sidePoints.push_back(newSp);

		for(auto& a : s->points){
			im.AddInfluence(a,newSp.me);
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
		influencerList.push_back(a.myEdge->p1); 
		influencerList.push_back(a.myEdge->p2); 
		for(auto& b : sidePoints){
			if(std::find(b.ePoints.begin(),b.ePoints.end(),a)!=b.ePoints.end()){
				influencerList.push_back(b.me);
				if(influencerList.size()==4)break;
			}
		}
		a.me->pos = Mesh::CalculateMidPoint(influencerList);
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
		std::vector<Mesh::Point*> tConns {te.myEdge->p1,te.myEdge->p2};
		myMesh->AddPoint(te.me,tConns);
	}
	int i = 0;
	for(auto& te : sidePoints){
		
		std::vector<Mesh::Point*> tConns;
		for(auto& tt : te.ePoints)tConns.push_back(tt.me);
		myMesh->AddPoint(te.me,tConns);
		
		for(unsigned int i = 0;i<te.ePoints.size();i++){
			
			unsigned int i1=i;
			unsigned int i2 =i+1;
			if(i2==te.ePoints.size())i2=0;
			Mesh::Point* commonP = NULL;
			if(te.ePoints[i1].myEdge->p1 == te.ePoints[i2].myEdge->p1) commonP = te.ePoints[i1].myEdge->p1;
			else if(te.ePoints[i1].myEdge->p1 == te.ePoints[i2].myEdge->p2) commonP = te.ePoints[i1].myEdge->p1;
			else if(te.ePoints[i1].myEdge->p2 == te.ePoints[i2].myEdge->p1) commonP = te.ePoints[i1].myEdge->p2;
			else if(te.ePoints[i1].myEdge->p2 == te.ePoints[i2].myEdge->p2) commonP = te.ePoints[i1].myEdge->p2;
			std::vector<Mesh::Point*> sss {commonP, te.ePoints[i1].me,te.me,te.ePoints[i2].me};
			
			myMesh->AddSide(sss);
			
		}

	}	
}

void OVertSubdivide::DeleteOldSidesAndEdges()
{
	for(auto& a : OriginalEdges){
		myMesh->DeleteEdge(a); Mesh::eBlock.Delete(a);
	}
	for(auto& a : OriginalSides){
		myMesh->DeleteSide(a); Mesh::sBlock.Delete(a);
	}
}

OVertSubdivide::EdgePoint::EdgePoint(Mesh::Edge* _edge) : myEdge(_edge)
{
	me = Mesh::pBlock.Allocate(Mesh::Point((_edge->p2->pos+_edge->p1->pos)/2));
}

OVertSubdivide::SidePoint::SidePoint(Mesh::Side* _side, std::vector<OVertSubdivide::EdgePoint> ep) : ePoints(ep)
{
	me = Mesh::pBlock.Allocate(Mesh::Point(Mesh::CalculateMidPoint(_side->points)));
	
}