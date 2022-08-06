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

//improve this
//from v1.
//expect: v1:v2 section contains p
inline float Interpolate(const vec2& v1, const vec2& v2, const vec2& p){
	return (p-v1).length() / (v2-v1).length();
}


#endif



