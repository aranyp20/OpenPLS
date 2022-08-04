#ifndef ADDONS_HPP
#define ADDONS_HPP
#include "InputManager.h"



class Operation : public InputBindable {


};

template<typename T>
bool VectorContains(std::vector<T*> where, T* what) 
{
	for (T* poss : where) {
		if (poss == what)return true;
	}
	return false;
}
#endif
