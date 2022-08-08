#ifndef SURFACE_H
#define SURFACE_H

#include "Mesh.h"
#include "Addons.hpp"
#include "Camera.h"



class Toloka : public InputBindable {

	struct Arrow {
		vec3 center;
		vec3 direction;
		vec3 color;

		Arrow(){}
		Arrow(vec3 _direction,vec3 _color, vec3 _center);

		void Update();

		void Replace(const vec3&);

		bool CheckHit(const vec2&, const mat4&);

		std::vector<float> GiveData();
	};


	bool active;
	vec3 center;

	std::vector<Mesh::Point*> controlledPoints; //pointernek kene lennie, hogy ha mozgatas kozben valtozik mennyi pont van akkor jo legyen
	Surface* owner;

	std::vector<Toloka::Arrow> arrows;
	Toloka::Arrow arrowActive;

	OVertMove* moveOperation;

	void Replace(const vec3&);

public:

	Toloka(Surface*);

	void Update();

	//controlledpointsal lesz
	void WakeUp(std::vector<Mesh::Point*>);
	void Sleep();


	bool CheckHit();

	std::vector<float> GiveData();
};

class Surface : public InputProcessor {
	
    GouraudShader shader;

	VAO* topLayerVAO;
	VBO* topLayerVBO;

public:
	NormalShader shader1;

	Camera* viewCamera;
	MeshHandler* meshHandler;
	Toloka *toloka;
	Light* light;

	//itt kell a camerat meg mh-t megcsin�lni
	Surface();



	InputAnswer ProcessKey(int key);

	void Render(Renderer&);

};



class OCameraMove : public Operation {
	Camera* camera;
public:
	OCameraMove(Camera*);

	void Update();
};

#endif