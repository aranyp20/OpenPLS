#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>

#include "mymath.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Addons.hpp"

class Surface;
class MeshRenderer;

class Hittable
{
public:
	struct Hit
	{
		float z;
		Hittable *hittable;
		Hit() : z(2), hittable(NULL) {}
	};
	virtual Hittable::Hit Intersect(const vec2 &, const mat4 &) = 0;
	virtual void NotifyWin() {}
};

class Mesh
{
public: // valamikor private lesz

	enum Shape{
		CUBE
	};

	struct Point : public Hittable
	{
		vec3 pos;
		Mesh* owner;

		inline Point(float x = 0, float y = 0, float z = 0) : pos(x, y, z),owner(NULL) {}

		Hittable::Hit Intersect(const vec2 &, const mat4 &);
		void NotifyWin();
	};
	struct Edge : public Hittable
	{
		Point *p1, *p2;
		Mesh* owner;

		inline Edge(Point *_p1, Point *_p2, Mesh* _owner) : p1(_p1), p2(_p2),owner(_owner) {}

		Hittable::Hit Intersect(const vec2 &, const mat4 &);
		void NotifyWin();
	};
	struct Side
	{
		std::vector<Point *> points;

		inline Side(std::vector<Point *> _ps) { points = _ps; }
	};

private:
	mat4 M;

	std::vector<Point *> points;
	std::vector<Point *> selectedPoints;

	std::vector<Side *> sides;

	bool corrupted;

	MeshRenderer* meshRenderer;

	struct EdgeMatrix
	{
		inline int Size() { return matrix.size(); }

		int elementIndex(Point *);
		Point *indexElement(int);

		std::vector<std::vector<Edge *>> matrix;

		// ennel gyorsabb es szebbet vagy legalabb osszeszervezni (nem is biztos hogy kell ez)
		std::vector<Point *> orderVec;
	};

	EdgeMatrix edgeMatrix;

	void SelectPoint(Point *);


	void TransformToCube();

	

public:
	struct EdgeIterator
	{
		EdgeIterator(Mesh &_parent, int _row, int _column);

		// EdgeIterator& operator++();

		Point *GetElement1();
		Point *GetElement2();

		Edge* GetElementEdge();

		bool hasNext();

	private:
		Mesh &parent;

		int row;
		int column;
	};
	Material* material;

	EdgeIterator begin();
	EdgeIterator end();

	void PrintVerts();

	Mesh(Mesh::Shape);

	void AddPoint(Point *vert, const std::vector<Point *> conns);
	void AddSide(std::vector<unsigned int> &);

	bool CheckHit(const vec2 &, const mat4 &);
	bool ReleaseSelection();
	void Transform(const mat4 &);

	bool Corrupted() { return corrupted; }
	void Corrupt() { corrupted = true; }
	void UnCorrupt() { corrupted = false; }

	std::vector<Mesh::Point*> GiveSelecteds();



	void Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
	friend class MeshRenderer;
};




//� birtokolja majd a vbo-kat
class MeshRenderer
{
	Mesh* owner;

	VAO* vertVAO;
	VAO* edgeVAO;
	VAO* sideVAO;
	VBO* vertVBO;
	VBO* edgeVBO;
	VBO* sideVBO;

	RenderData GiveSides();
	std::vector<float> GiveVertices();
	std::vector<float> GiveEdges();
public:
	

	MeshRenderer(Mesh*);

	void Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
};

class MeshHandler : public InputProcessor
{
	std::vector<Mesh *> meshes;
	Mesh *activeMesh;

	Surface *owner;

public:
	MeshHandler(Surface *);

	InputAnswer ProcessKey(int key);
	void AddMesh(Mesh *);
	bool CheckHit(const vec2 &);
	void Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
};

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

//OVertMove nem lenne rossz
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

#endif