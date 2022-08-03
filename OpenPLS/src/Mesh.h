#pragma once

#include <vector>
#include <map>


#include "mymath.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Addons.hpp"

class Surface;

class Hittable {
public:
	struct Hit {
		float z;
		Hittable* hittable;
		Hit():z(2),hittable(NULL){}
	};
	virtual Hittable::Hit Intersect(const vec2&) = 0;
};




class Mesh
{
public://valamikor private lesz
	struct Point
	{
		vec3 pos;

		inline Point(float x = 0, float y = 0, float z = 0) : pos(x, y, z) { }


	};
	struct Edge
	{
		Point* p1,* p2;

		inline Edge(Point* _p1, Point* _p2) : p1(_p1), p2(_p2){}

		
	};
	struct Side
	{
		std::vector<Point*> points;

		inline Side(std::vector<Point*> _ps) { points = _ps; }

	};
private:
	


	mat4 M;


	std::vector<Point*> points;
	std::vector<Point*> selectedPoints;

	bool corrupted;

	struct EdgeMatrix
	{
		inline int Size() { return matrix.size(); }

		int elementIndex(Point*);
		Point* indexElement(int);
		
		
		std::vector<std::vector<int>> matrix;

		//ennel gyorsabb es szebbet vagy legalabb osszeszervezni
		std::vector<Point*> orderVec;
		

	};

	EdgeMatrix edgeMatrix;



	std::vector<Side> sides;

	
	
public:
	struct EdgeIterator
	{
		EdgeIterator(Mesh& _parent, int _row,int _column);

		

		//EdgeIterator& operator++();

		Point* GetElement1();
		Point* GetElement2();

		bool hasNext();

	private:
		Mesh& parent;

		int row;
		int column;
	};
	EdgeIterator begin();
	EdgeIterator end();

	void PrintVerts();

	Mesh();

	void AddPoint(Point* vert, const std::vector<Point*> conns);
	void AddSide(std::vector<unsigned int>&);

	bool CheckHit(const vec2&, const mat4&);
	bool ReleaseSelection();
	void Transform(const mat4&);


	bool Corrupted() { return corrupted; }
	void Corrupt() { corrupted = true; }
	void UnCorrupt() { corrupted = false; }
	friend class MeshRenderer;
};

//õ birtokolja majd a vbo-kat
class MeshRenderer
{

public:

	static RenderData GiveSides(Mesh&);//private lesz
	static std::vector<float> GiveVertices(Mesh&);
	static VBO* GiveEdges(Mesh&);

	static void RenderThisMesh(Renderer&, VAO&,Shader&, Mesh&);


};

class MeshHandler : public InputProcessor {
	 std::vector<Mesh*> meshes;
	 Mesh* activeMesh;

	 Surface* owner;
public:
	MeshHandler(Surface*);

	InputAnswer ProcessKey(int key);
	void AddMesh(Mesh*);
	bool CheckHit(const vec2&);
};

class OVertMove : public Operation {
	Surface* surface;
	vec3 direction;
	vec3 startingPosition;
public:
	OVertMove(Surface*,vec3 dir,vec3 sp);

	void Update();
};
