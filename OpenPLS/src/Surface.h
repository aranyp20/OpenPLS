#ifndef SURFACE_H
#define SURFACE_H

#include "Mesh.h"
#include "Addons.hpp"
#include "Camera.h"

class Surface : public InputProcessor {
	

public:
	Camera* viewCamera;
	MeshHandler* meshHandler;

	//itt kell a camerat meg mh-t megcsin�lni
	Surface(){}



	InputAnswer ProcessKey(int key);

};

class Toloka : public InputBindable {

public:

};

class OCameraMove : public Operation {
	Camera* camera;
public:
	OCameraMove(Camera*);

	void Update();
};

#endif