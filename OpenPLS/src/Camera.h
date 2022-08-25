#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include "mymath.h"




class Camera
{
	vec3 wEye, wUp, wLookat;
	float fov, aspect, fp, bp;

	float originalDistance;
	vec3 lastLookDir;
	
	void RecalculateLastLookDir();

public:
	Camera();
	mat4 V() const;
	mat4 P() const;
	void Replace(vec3 _wEye,vec3 _wLookat);
	void ReplaceEye(vec3 _wEye);
	void ReplaceLookat(vec3 _wLookat);

	std::vector<vec3> GetWUV() const;

	inline vec3 GetEye() const { return wEye; }
	inline vec3 GetLookat() const { return wLookat; }
	inline float GetDepth() const {return fp; }
	inline float GetOriginalDistance() const {return originalDistance;}
	inline vec3 GetLastLookDir() const {return lastLookDir;}

	vec3 PutToWorld(const vec2&);
};


#endif