#pragma once
#include "VBO.h"



class VAO
{
private:
	unsigned int ID;
public:
	VAO();
	~VAO();

	void AddVBO(const VBO& vb);

	void Bind() const;

};
