#pragma once
#include "VBO.h"



class VAO
{
private:
	unsigned int ID;
	unsigned int count;
public:
	VAO();
	~VAO();

	void AddVBO(const VBO& vb);

	void Bind() const;
	unsigned int GetCount() const { return count; }
};
