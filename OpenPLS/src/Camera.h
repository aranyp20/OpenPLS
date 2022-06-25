#pragma once
#include "mymath.h"



class Camera
{
	vec3 wEye, wUp, wLookat;
	float fov, aspect, fp, bp;

public:
	Camera();
	mat4 V();
	mat4 P();
	void Replace(vec3 _wEye,vec3 _wLookat);
	void ReplaceEye(vec3 _wEye);
	void ReplaceLookat(vec3 _wLookat);
};