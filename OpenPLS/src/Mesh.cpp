#include <algorithm>

#include "Mesh.h"
#include "Surface.h"
#include "Addons.hpp"

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
	corrupted = false;
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

bool Mesh::CheckHit(const vec2& p, const mat4& MVP)
{
	//ez a mesh intersectje legyen, ami ebben van az kb megy a Point intersecjebe

	float closestZ = 2.0f;
	Point* closestHit = NULL;
	vec2 pp = InputManager::ChangeInput(p);

	for (int i = 0; i < points.size(); i++) {
		vec3 vpos = points[i]->pos;
		TransformPoint(vpos, MVP);
		vpos = vpos ;
		vec2 spos = vec2(vpos.x, vpos.y);
		
		if (vpos.z<1 && vpos.z>-1 && closestZ > vpos.z && (pp - spos).length() < 0.015f) {
			closestHit = points[i];
			closestZ = vpos.z;
		}
	}
	if (closestHit != NULL) {
		selectedPoints.push_back(closestHit);
		Corrupt();
	}
	return (closestHit!=NULL);
}

bool Mesh::ReleaseSelection()
{
	if (!selectedPoints.empty()) {
		selectedPoints.clear();
		Corrupt();
		return true;
	}
	return false;
}

void Mesh::Transform(const mat4& m)
{
	M = M * m;
}


Mesh::EdgeIterator::EdgeIterator(Mesh& _parent, int _row = 0, int _column = 0):parent(_parent),row(_row),column(_column)
{

}

Mesh::EdgeIterator Mesh::begin()
{
	return EdgeIterator(*this);
}

//nem valoszinu hogy jo
//Mesh::EdgeIterator Mesh::EdgeIterator::end()
//{
//	return EdgeIterator(parent, parent.edgeMatrix.Size()-1,parent.edgeMatrix.Size()-2);
//}

Mesh::Point* Mesh::EdgeIterator::GetElement1()
{
	return parent.edgeMatrix.indexElement(row);
}

Mesh::Point* Mesh::EdgeIterator::GetElement2()
{
	return parent.edgeMatrix.indexElement(column);
}

bool Mesh::EdgeIterator::hasNext()
{
	

	do {
		int highestValidRow = column;
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
	} while (parent.edgeMatrix.matrix[column][row] == 0);
	
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

Mesh::Point* Mesh::EdgeMatrix::indexElement(int i)
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

std::vector<float> MeshRenderer::GiveVertices(Mesh& m)
{
	std::vector<float> res;
	for (Mesh::Point* p : m.points) {
		res.push_back(p->pos.x);
		res.push_back(p->pos.y);
		res.push_back(p->pos.z);
		if (VectorContains<Mesh::Point>(m.selectedPoints, p)) {
			res.push_back(0); res.push_back(0); res.push_back(1);
		}
		else {
			res.push_back(0); res.push_back(1); res.push_back(0);
		}
		
	}

	return res;
}

VBO* MeshRenderer::GiveEdges(Mesh& m)
{
	std::vector<float> res;
	Mesh::EdgeIterator it = m.begin();
	while (it.hasNext()) {
		res.push_back(it.GetElement1()->pos.x);
		res.push_back(it.GetElement1()->pos.y);
		res.push_back(it.GetElement1()->pos.z);
		res.push_back(1); res.push_back(0); res.push_back(0);
		res.push_back(it.GetElement2()->pos.x);
		res.push_back(it.GetElement2()->pos.y);
		res.push_back(it.GetElement2()->pos.z);
		res.push_back(1); res.push_back(0); res.push_back(0);
	}
	return new VBO3f3f(res);
}

void MeshRenderer::RenderThisMesh(Renderer& renderer,VAO& vao,Shader& shader, Mesh& mesh)
{
	RenderData rd = GiveSides(mesh);
	//vao.AddVBO(*(rd.vbo));
	//renderer.Draw(Renderer::TriangleData(vao, *(rd.ibo), shader));
}

MeshHandler::MeshHandler(Surface* s) : owner(s)
{
}

InputAnswer MeshHandler::ProcessKey(int key)
{
	if (key == GLFW_KEY_W && CheckHit(InputManager::GetMousePos2())) {
		return InputAnswer(InputAnswer::ReactionType::PROCESSED, NULL);
	}



	return InputAnswer(InputAnswer::ReactionType::IGNORED,NULL);
}

void MeshHandler::AddMesh(Mesh* m)
{
	activeMesh = m;
	meshes.push_back(m);
}

bool MeshHandler::CheckHit(const vec2& p)
{
	
	if (activeMesh->CheckHit(p, owner->viewCamera->V() * owner->viewCamera->P())) {
		return true; activeMesh->Corrupt();
	}
	return activeMesh->ReleaseSelection();
}

OVertMove::OVertMove(Surface* surf, vec3 dir, vec3 sp) : surface(surf), direction(dir), startingPosition(sp)
{
}

void OVertMove::Update()
{
}
