#include "VBO.h"
#include "Renderer.h"

VBO::sizeMap VBO::sm = { { GL_FLOAT,4 }, { GL_UNSIGNED_INT,4 } };

VBO::VBO(const void* data, unsigned int size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &ID);
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}



void VBO2f::TellData() const
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sm[GL_FLOAT]*2, nullptr);
	glEnableVertexAttribArray(0);

}
void VBO3f3f::TellData() const
{
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sm[GL_FLOAT] * 6, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sm[GL_FLOAT] * 6, (GLvoid*)(3*sm[GL_FLOAT]));
	glEnableVertexAttribArray(1);
}
