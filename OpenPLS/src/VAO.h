#ifndef VAO_H
#define VAO_H

#include "VBO.h"



class VAO
{
private:
	unsigned int ID;
	
public:
	unsigned int count; //kell egy map hogy melyik vbo mennyit tarol, most igy nem jo es ez nyilvan priv lesz

	VAO();
	~VAO();

	void AddVBO(VBO& vb);

	void Bind() const;
	unsigned int GetCount() const { return count; }
};


#endif