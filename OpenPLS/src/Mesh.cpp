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


