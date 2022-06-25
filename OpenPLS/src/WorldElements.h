#pragma once
#include "mymath.h"

struct Material
{
	vec3 kd, ks, ka;
	float shininess;

};

struct Light
{
	vec3 La, Le;
	vec4 wLightPos;
};