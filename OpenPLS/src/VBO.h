#pragma once
#include <vector>
#include <map>
#include "GL/glew.h"


class VBO
{
private:
	const void* myData;
protected:
	unsigned int ID;
	typedef std::map<unsigned int, unsigned int> sizeMap;
	static sizeMap sm;
public:
	VBO(const void* data, unsigned int size);
	VBO(const std::vector<float>&);
	~VBO();
	virtual void TellData() const =0 ;
	void Bind() const;
	
};
//Vertex buffer containing: 2 float / data
class VBO2f : public VBO
{
public:
	VBO2f(const void* data, unsigned int size) : VBO(data,size){}
	void TellData() const;
};
//Vertex buffer containing: 3*2 float / data
class VBO3f3f : public VBO
{
public:
	VBO3f3f(const void* data, unsigned int size) : VBO(data, size) {}
	VBO3f3f(const std::vector<float>& v) : VBO(v) {}
	void TellData() const;
};
