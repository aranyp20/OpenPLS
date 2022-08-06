#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>

#include "mymath.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Addons.hpp"

class Surface;

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
	EdgeIterator begin();
	EdgeIterator end();

	void PrintVerts();

	Mesh();

	void AddPoint(Point *vert, const std::vector<Point *> conns);
	void AddSide(std::vector<unsigned int> &);

	bool CheckHit(const vec2 &, const mat4 &);
	bool ReleaseSelection();
	void Transform(const mat4 &);

	bool Corrupted() { return corrupted; }
	void Corrupt() { corrupted = true; }
	void UnCorrupt() { corrupted = false; }

	std::vector<Mesh::Point*> GiveSelecteds();

	friend class MeshRenderer;
};

//� birtokolja majd a vbo-kat
class MeshRenderer
{

public:
	static RenderData GiveSides(Mesh &); // private lesz
	static std::vector<float> GiveVertices(Mesh &);
	static std::vector<float> GiveEdges(Mesh &);

	static void RenderThisMesh(Renderer &, VAO &, Shader &, Mesh &);
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
};

/* class OVertMove : public Operation
{
	Surface *surface;
	vec3 direction;
	vec3 startingPosition;

public:
	OVertMove(Surface *, vec3 dir, vec3 sp);

	void Update();
}; */


#endif