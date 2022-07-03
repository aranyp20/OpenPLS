#pragma once

#include <vector>
#include <map>


#include "mymath.h"


struct Point
{
	inline Point(int x = 0, int y = 0):pos(x,y){}

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
		

	


		//ennel gyorsabb es szebbet vagy legalabb osszeszervezni
		std::vector<std::vector<int>> matrix;
		std::vector<Point*> orderVec;
		

	};

	EdgeMatrix edgeMatrix;
	
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
	

};