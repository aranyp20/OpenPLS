#ifndef SURFACE_H
#define SURFACE_H

#include "Mesh.h"
#include "MeshOperation.h"
#include "Addons.hpp"
#include "Camera.h"
#include "Geometry.h"



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
	Shader2D shader2D;

	VAO* topLayerVAO;
	VBO* topLayerVBO;

	VAO* topLayerVAO2D;
	VBO* topLayerVBO2D;

	//ezt lehetne templatesen altalanosan addons-ba
	std::map<int,std::vector<float>> outsideRenderData;
public:
	int RequestDataSpace();
	void FillData(int, std::vector<float>&);
	void FreeDataSpace(int);



	NormalShader shader1;

	static Camera* viewCamera;
	MeshHandler* meshHandler;
	Toloka *toloka;
	Light* light;

	Surface();



	InputAnswer ProcessKey(int key);
	InputAnswer ProcessMouseClick();

	void Render(Renderer&);
	

};



class OCameraMove : public Operation {
	Camera* camera;
public:
	OCameraMove(Camera*);

	void Update();
};

class OCameraFocusSet : public Operation {
	Camera* camera;

public:
	OCameraFocusSet(Camera*,float);
	void Update();

};

class OBoxSelection : public Operation{
	Surface* owner;
	int dataSpaceIndex;
	
	vec2 startingPos;
	Rect selRect;
public:
	OBoxSelection(Surface*);
	~OBoxSelection() override;
	void Update() override;
	void Release() override;
};

#endif