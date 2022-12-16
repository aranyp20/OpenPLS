#ifndef MESHOPERATION_H
#define MESHOPERATION_H

#include "Mesh.h"




class MeshOperation : public Operation{
	protected:
	std::vector<Mesh::Point*> controlledPoints;
	vec2 startingPos;
	vec3 midPoint;

	void FindMidPoint();

public:

	MeshOperation(Mesh*);
	
	virtual void Update() = 0;

};

class OVertScale : public MeshOperation{
	std::vector<vec3> originalDefficit;
	public:
	OVertScale(Mesh*);
	void Update();
};

class OVertRotate : public MeshOperation{
	vec3 axis;
	public:
	OVertRotate(Mesh*,vec3);
	void Update();

};

class OVertMove : public MeshOperation{
	vec3 axis;
	Camera* camera;
	public:
	OVertMove(Mesh*,vec3,Camera*);
	void Update();
	vec3 GetMidPoint();
};

class OVertExtrude : public MeshOperation{

	public:
	OVertExtrude(Mesh*);
	void Update();

};

//infmappal megcsinalni az edgepointot is
class OVertSubdivide : public MeshOperation{
	Mesh* myMesh;

	struct InfluenceMap{
		std::map<Mesh::Point*,std::vector<Mesh::Point*>> infMap;
		
		void AddInfluence(Mesh::Point* for_what,Mesh::Point* what);
	};


	struct EdgePoint {
		Mesh::Point* me;
		Mesh::Edge* myEdge;
		EdgePoint(Mesh::Edge*);

		bool operator== (const EdgePoint& rs){return me==rs.me;}
	};

	struct SidePoint {
		Mesh::Point* me;
		std::vector<EdgePoint> ePoints;
		SidePoint(Mesh::Side*,std::vector<EdgePoint>);

		bool operator== (const SidePoint& rs){return me==rs.me;}
	};

	InfluenceMap im;

	std::vector<EdgePoint> edgePoints;
	std::vector<SidePoint> sidePoints;
	std::vector<Mesh::Point*> oldPoints;

	std::vector<Mesh::Edge*> OriginalEdges;
	std::vector<Mesh::Side*> OriginalSides;

	void SnapShot();

	void FillEdgePoints();
	void FillSidePoints();
	void FillOldPoints();

	void ReplaceEdgePoints();
	void ReplaceOldPoints();

	//static valtozoban el lehetne tarulni a kulonbozo szintu halokat es tolokaval valtoztatni, hogy melyik legyen
	void CreateNewMesh();
	void DeleteOldSidesAndEdges();

public:

	OVertSubdivide(Mesh*);
	~OVertSubdivide();


	void Update(){}

};





























#endif