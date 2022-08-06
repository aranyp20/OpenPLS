#ifndef VBO_H
#define VBO_H

#include <vector>
#include <map>
#include <GL/glew.h>

class VAO;

class VBO
{
private:
	const void* myData;
	unsigned int count;
	VAO* owner;
protected:
	unsigned int ID;
	typedef std::map<unsigned int, unsigned int> sizeMap;
	static sizeMap sm;
public:
	VBO(const void* data, unsigned int size);
	//ezt templatesen is lehetne
	VBO(const std::vector<float>&);
	
	~VBO();
	virtual void TellData() const =0 ;
	unsigned int GetCount() const { return count; }
	void Bind() const;
	void RefreshData(const std::vector<float>&);
	void SetOwner(VAO* o){owner =o;}
};
//Vertex buffer containing: 2 float / data
class VBO2f : public VBO
{
public:
	VBO2f(const void* data, unsigned int size) : VBO(data,size){}
	void TellData() const;
};

class VBO3f : public VBO
{
public:
	VBO3f(const void* data, unsigned int size) : VBO(data, size) {}
	VBO3f(const std::vector<float>& v) : VBO(v) {}
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

#endif