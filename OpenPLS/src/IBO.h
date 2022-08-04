#ifndef IBO_H
#define IBO_H

#include <vector>

class IBO
{
private:
	unsigned int ID;
	unsigned int count;
public:
	IBO(const unsigned int* data, unsigned int _count);
	IBO(std::vector<unsigned int>&);
	~IBO();
	void Bind() const;
	inline unsigned int GetCount() const { return count; }
};

#endif