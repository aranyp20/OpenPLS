#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>

#include "mymath.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Addons.hpp"
#include "Camera.h"


#define MAX_MESH_POINT 10000


class Surface;
class MeshRenderer;
class Rect;

	

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
		inline Point(const vec3& v) : pos(v.x,v.y,v.z), owner(NULL){}
		inline Point(const Point& p) : pos(p.pos), owner(p.owner) {}


		Hittable::Hit Intersect(const vec2 &, const mat4 &);
		void NotifyWin();
	};
	struct Edge : public Hittable
	{
		Point *p1, *p2;
		Mesh* owner;

		inline Edge(Point *_p1 = NULL, Point *_p2 = NULL, Mesh* _owner = NULL) : p1(_p1), p2(_p2),owner(_owner) {}

		Hittable::Hit Intersect(const vec2 &, const mat4 &);
		void NotifyWin();
	};
	struct Side
	{
		std::vector<Point *> points;

		inline Side(std::vector<Point *> _ps) { points = _ps; }
		inline Side(){}

		std::vector<Edge*> GetEdges(Mesh*);
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

		unsigned int elementIndex(Point *);
		Point *indexElement(int);

		Edge* GetRelationshipBetweenPoints(Point*,Point*);
		void SetRelationshipBetweenPoints(Point*,Point*,Edge*);

		std::vector<std::vector<Edge *>> matrix;

		// ennel gyorsabb es szebbet vagy legalabb osszeszervezni (nem is biztos hogy kell ez)
		std::vector<Point *> orderVec;
	};

	EdgeMatrix edgeMatrix;

	void SelectPoint(Point *);
	void TransformToCube();




	

public:
	static mem_block<Mesh::Point> pBlock;
	static mem_block<Mesh::Edge> eBlock;
	static mem_block<Mesh::Side> sBlock;

	enum Mode{
		OBJECT, EDIT, VERTEX
	};

	void ChangeMode(const Mode&);

	//az nem tul jo hogy mindig mindket oldalrol meg kell vizsgalni a dolgokat
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


	Mesh(Mesh::Shape);

	void AddPoint(Point *vert, const std::vector<Point *> conns);
	void AddPoint(Point *vert);
	void AddSide(std::vector<unsigned int> &);
	void AddSide(std::vector<Point*>&);
	void AddSideExtra(std::vector<Point*>);
	//ezek csinalhatnak a delete operationt
	void DeleteSide(Side*);
	void DeleteEdge(Edge*);

	bool CheckHit(const vec2 &, const mat4 &);
	bool ReleaseSelection();
	void Transform(const mat4 &);
	void SelectPoints(const Rect&,const Camera*);

	bool Corrupted() { return corrupted; }
	void Corrupt() { corrupted = true; }
	void UnCorrupt() { corrupted = false; }

	std::vector<Mesh::Point*> GiveSelecteds();

	static vec3 CalculateMidPoint(std::vector<Point*>);

private:
	struct Strategy{
		virtual void Render(MeshRenderer*,const Renderer& r, const Shader& vs,const Shader& es,Shader& ss){}
		virtual bool CheckHit(Mesh*,const vec2 &, const mat4 &){return false;}
		bool CheckHitCommon(Mesh*,const vec2 &, const mat4 &,std::vector<Hittable*>&);
	};

	struct StrategyEditMode : public Strategy{
		void Render(MeshRenderer*,const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
		bool CheckHit(Mesh*,const vec2 &, const mat4 &);
	};
	StrategyEditMode stratEDIT;
	struct StrategyObjectMode : public Strategy{
		void Render(MeshRenderer*,const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
		bool CheckHit(Mesh*,const vec2 &, const mat4 &);
	};
	StrategyObjectMode stratOBJ;
	struct StrategyVertexMode : public Strategy{
		void Render(MeshRenderer*,const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
		bool CheckHit(Mesh*,const vec2 &, const mat4 &);
	};
	StrategyVertexMode stratVERTEX;

	Strategy* currentStrat;
public:

	void Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
	

	friend class MeshRenderer;
	friend class OVertExtrude;
	friend class OVertSubdivide;
};





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

	void RenderAsEdit(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
	void RenderAsObject(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
	void RenderAsVertex(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);
};

class MeshHandler : public InputProcessor
{



	std::vector<Mesh *> meshes;
	Surface *owner;

public:

	

	static Mesh *activeMesh;
	MeshHandler(Surface *);

	InputAnswer ProcessKey(int key);
	InputAnswer ProcessMouseClick();
	void AddMesh(Mesh *);
	void ChangeMode(Mesh::Mode);
	bool CheckHit(const vec2 &);
	void Render(const Renderer& r, const Shader& vs,const Shader& es,Shader& ss);

	void SelectPoints(const Rect&);
};


#endif