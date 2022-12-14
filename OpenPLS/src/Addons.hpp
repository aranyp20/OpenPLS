#ifndef ADDONS_HPP
#define ADDONS_HPP
//#include "InputManager.h"

#include <vector>
#include <map>
#include "mymath.h"

class mem_alloc_exception : public std::exception{
	public:
	mem_alloc_exception(){
		std::cout<<"Not enough memory in block"<<std::endl;
	}
};

//Invokes operator=  not copy-constructor!!
template<typename T>
class mem_block{
	unsigned int size;
	unsigned int usedSize = 0;
	T* block;
	
	std::map<unsigned int,bool> occupationMap;



	public:
	mem_block(unsigned int _size) : size(_size){
		block = new T[_size];
	}
	T* Allocate(const T& data){

		for(int i=0;i<usedSize;i++){
			if(!occupationMap[i]){
				occupationMap[i] = true;
				block[i] = data;
				return block + i;
			}
		}
		if(usedSize<size){
			occupationMap.insert({usedSize,true});
			block[usedSize] = data;
			usedSize++;
			return block + usedSize - 1;
		}
		throw mem_alloc_exception();
	}
	void Delete(T* deletable){
		
		unsigned int deleteIndex = deletable - block;
		
		occupationMap[deleteIndex] = false;
	}

	~mem_block(){
		delete[] block;
	}

};


template<typename T>
class DataSpace{

	std::map<int,T> data;

public:

	int RequestDataSpace()
	{
		std::vector<int> keys;
		int maxID=0;
		for(auto it = data.begin();it !=data.end();it++){
			keys.push_back(it->first);
			maxID = std::max(maxID,it->first);
		}
		bool idReservations[maxID+2];
		for(int j = 0 ;j<maxID+2;j++){
			idReservations[j] = false;
		}

		for(auto a : keys){
			idReservations[a] = true;
		}
		int i = 0;
		while(idReservations[i])i++;
		T newSpace;
		data.insert({i,newSpace});
		return i;
	}

	void FillData(int id, T& _data)
	{
		data[id] = _data;
	}

	void FreeDataSpace(int id)
	{
		data.erase(id);
	}

	std::vector<T> GetData()
	{
		std::vector<T> result;

		for(auto it = data.begin();it !=data.end();it++){
			result.push_back(it->second);
		}
		return result;
	}


};



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



inline void PushBack(std::vector<float>& where,const vec3 what)
{
	where.push_back(what.x); where.push_back(what.y); where.push_back(what.z);
}

inline void PushBack(std::vector<float>& where, const std::vector<vec3>& what)
{
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(a.z);
	}
}

inline void PushBack(std::vector<float>& where, const std::vector<vec2>& what, float _z)
{
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(_z);
	}
}

inline void PushBack(std::vector<float>& where, const std::vector<vec3>& what, const vec3 repeat)
{
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(a.z);
		PushBack(where,repeat);
	}
}
inline void PushBack(std::vector<float>& where, const std::vector<vec2>& what,const vec3 repeat, float _z)
{
	
	for(auto& a : what){
		where.push_back(a.x); where.push_back(a.y); where.push_back(_z);
		PushBack(where,repeat);
	}
}




#endif



