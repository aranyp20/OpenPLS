#include "VAO.h"
#include "Renderer.h"


VAO::VAO()
{
	count = 0;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
	Bind();
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &ID);
}

void VAO::AddVBO(VBO& vb)
{
	Bind();
	vb.SetOwner(this);
	vb.Bind();
	vb.TellData();
	count += vb.GetCount();
}

void VAO::Bind() const
{
	glBindVertexArray(ID);
}


