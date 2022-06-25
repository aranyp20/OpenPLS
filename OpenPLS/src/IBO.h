#pragma once

class IBO
{
private:
	unsigned int ID;
	unsigned int count;
public:
	IBO(const unsigned int* data, unsigned int _count);
	~IBO();
	void Bind() const;
	inline unsigned int GetCount() const { return count; }
};
