#ifndef SURFACE_H
#define SURFACE_H

#include "Mesh.h"
#include "Addons.hpp"
#include "Camera.h"


class Toloka : public InputBindable {

	struct Arrow : public InputBindable{
		vec3 center;
		vec3 direction;
		vec3 color;

		
		Arrow(vec3 _direction,vec3 _color, vec3 _center);

		void Update();

		void Replace(const vec3&);

		std::vector<float> GiveData();
	};


	bool active;
	vec3 center;

	std::vector<Mesh::Point*> controlledPoints;
	Surface* owner;

	Toloka::Arrow arrowX, arrowY,arrowZ; //lehet ezeket inkabb tombben kene
	Toloka::Arrow* arrowActive;

public:

	Toloka(Surface*);

	void Update();

	//controlledpointsal lesz
	void WakeUp(std::vector<Mesh::Point*>);
	void Sleep();

	std::vector<float> GiveData();
};

class Surface : public InputProcessor {

public:
	Camera* viewCamera;
	MeshHandler* meshHandler;

	Toloka toloka;

	//itt kell a camerat meg mh-t megcsin�lni
	Surface():toloka(this){}



	InputAnswer ProcessKey(int key);


};



class OCameraMove : public Operation {
	Camera* camera;
public:
	OCameraMove(Camera*);

	void Update();
};

#endif