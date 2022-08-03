#include "VBO.h"
#include "Renderer.h"

VBO::sizeMap VBO::sm = { { GL_FLOAT,4 }, { GL_UNSIGNED_INT,4 } };

VBO::VBO(const void* data, unsigned int size)
{
	myData = data;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VBO::VBO(const std::vector<float>& v)
{
	count = v.size();
	float* dataT = new float[v.size()];
	for (int i = 0; i < v.size();i++) {
		dataT[i] = v[i];
	}

	myData = dataT;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), dataT, GL_DYNAMIC_DRAW);
}



VBO::~VBO()
{
	
	glDeleteBuffers(1, &ID);

	free((float*)myData);
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::RefreshData(const std::vector<float>& v)
{

	count = v.size();
	float* dataT = new float[v.size()];
	for (int i = 0; i < v.size(); i++) {
		dataT[i] = v[i];
	}

	myData = dataT;
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0,v.size() * sizeof(float), dataT);
}



void VBO2f::TellData() const
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sm[GL_FLOAT] * 2, nullptr);
	glEnableVertexAttribArray(0);

}
void VBO3f3f::TellData() const
{
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sm[GL_FLOAT] * 6, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sm[GL_FLOAT] * 6, (GLvoid*)(3*sm[GL_FLOAT]));
	glEnableVertexAttribArray(1);
}

void VBO3f::TellData() const
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sm[GL_FLOAT] * 3, nullptr);
	glEnableVertexAttribArray(0);
}
