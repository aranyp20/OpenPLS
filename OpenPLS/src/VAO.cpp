#include "VAO.h"
#include "Renderer.h"


VAO::VAO()
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
	Bind();
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &ID);
}

void VAO::AddVBO(const VBO& vb)
{
	vb.Bind();
	vb.TellData();

}

void VAO::Bind() const
{
	glBindVertexArray(ID);
}


