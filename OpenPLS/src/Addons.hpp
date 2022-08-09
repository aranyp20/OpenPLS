#ifndef ADDONS_HPP
#define ADDONS_HPP
//#include "InputManager.h"

#include <vector>
#include "mymath.h"






template<typename T>
bool VectorContains(std::vector<T*> where, T* what) 
{
	for (T* poss : where) {
		if (poss == what)return true;
	}
	return false;
}


//ennek egy konnyebben peldanyosíthato formajat kene
template<typename T, typename F>
struct VectorPair{
	struct InitMethod{
		virtual F InitFun(T);
	};

	std::vector<T> mainValue;
	std::vector<F> sideValue;

	VectorPair(T, F,InitMethod& im){
		for(T mv : mainValue){
			sideValue.push_back(im(mv));
		}
	}
};

//improve this
//from v1.
//expect: v1:v2 section contains p
inline float Interpolate(const vec2& v1, const vec2& v2, const vec2& p){
	return (p-v1).length() / (v2-v1).length();
}

inline void PushBack(std::vector<float>& where,vec3 what)
{
	where.push_back(what.x); where.push_back(what.y); where.push_back(what.z);
}


#endif



