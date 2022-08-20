#ifndef ADDONS_HPP
#define ADDONS_HPP
//#include "InputManager.h"

#include <vector>
#include "mymath.h"





//ehelyett std::find
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

inline void PushBack(std::vector<float>& where, std::vector<vec3>& what)
{
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(a.z);
	}
}

inline void PushBack(std::vector<float>& where, std::vector<vec2>& what, float _z)
{
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(_z);
	}
}

inline void PushBack(std::vector<float>& where, std::vector<vec3>& what, vec3 repeat)
{
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(a.z);
		PushBack(where,repeat);
	}
}
inline void PushBack(std::vector<float>& where, std::vector<vec2>& what,vec3 repeat, float _z)
{
	
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(_z);
		PushBack(where,repeat);
	}
}



#endif



