#include <algorithm>

#include "Mesh.h"


void Mesh::PrintVerts() 
{
	std::cout << "Matrix is:\n";

	for (int i = 0; i < edgeMatrix.Size(); i++) {
		for (int j = 0; j < edgeMatrix.matrix[i].size(); j++) {
			std::cout << edgeMatrix.matrix[i][j];
		}
		std::cout << "\n";
	}
}

Mesh::Mesh()
{
}



void Mesh::AddPoint(Point* vert, const std::vector<Point*> conns)
{

	std::vector<int> newColumn(edgeMatrix.Size() + 1);


	for (Point* c : conns) {
		newColumn[edgeMatrix.elementIndex(c)] = 1;
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
	sides.push_back(res);
}


Mesh::EdgeIterator::EdgeIterator(Mesh& _parent, int _row = 0, int _column = 0):parent(_parent),row(_row),column(_column)
{

}

Mesh::EdgeIterator Mesh::EdgeIterator::begin()
{
	return EdgeIterator(parent);
}

//nem valoszinu hogy jo
Mesh::EdgeIterator Mesh::EdgeIterator::end()
{
	return EdgeIterator(parent, parent.edgeMatrix.Size()-1,parent.edgeMatrix.Size()-2);
}

bool Mesh::EdgeIterator::hasNext()
{
	int highestValidRow = column - 1;
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

Point* Mesh::EdgeMatrix::indexElement(int i)
{
	return orderVec[i];
}

RenderData MeshRenderer::GiveSides(Mesh& m)
{
	RenderData res;
	
	std::vector<float> f;
	std::vector<unsigned int> inds;
	

	unsigned int iHelp = 0;

	for (Mesh::Side s : m.sides) {
		if (s.points.size() == 4) {
			vec3 norm = cross(s.points[1]->pos - s.points[0]->pos, s.points[3]->pos - s.points[0]->pos);

			for (int i = 0; i < 4; i++) {
				f.push_back(s.points[i]->pos.x);
				f.push_back(s.points[i]->pos.y);
				f.push_back(s.points[i]->pos.z);
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

			iHelp++;
		}
	}

	res.ibo = new IBO(inds);
	res.vbo = new VBO3f3f(f);

	return res;
}
