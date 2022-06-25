#include "IBO.h"
#include "Renderer.h"

IBO::IBO(const unsigned int* data, unsigned int _count) : count(_count)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	Bind();
}

IBO::~IBO()
{
	glDeleteBuffers(1, &ID);
}

void IBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

