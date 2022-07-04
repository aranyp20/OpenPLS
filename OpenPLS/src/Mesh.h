#pragma once

#include <vector>
#include <map>


#include "mymath.h"
#include "Renderer.h"


struct Point
{
	inline Point(float x = 0, float y = 0, float z = 0) : pos(x, y, z) { }

	vec3 pos;
};

class Mesh
{
private:
	std::vector<Point*> points;

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

	struct Side
	{
		inline Side(std::vector<Point*> _ps) { points = _ps; }

		std::vector<Point*> points;
	};

	std::vector<Side> sides;

	
public:
	struct EdgeIterator
	{
		EdgeIterator(Mesh& _parent, int _row,int _column);

		EdgeIterator begin();
		EdgeIterator end();

		//EdgeIterator& operator++();

		bool hasNext();

	private:
		Mesh& parent;

		int row;
		int column;
	};

	void PrintVerts();

	Mesh();

	void AddPoint(Point* vert, const std::vector<Point*> conns);
	void AddSide(std::vector<unsigned int>&);



	friend class MeshRenderer;
};


class MeshRenderer
{



public:

	static RenderData GiveSides(Mesh&);


};