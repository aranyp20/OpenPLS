#include "Camera.h"
#include "Program.h"

Camera::Camera()
{
	wLookat = vec3(0, 0, 0);
	wUp = vec3(0, 1, 0);
	wEye = vec3(-1, 0, 0);



	aspect = (float)(Program::GetInstance().WindowWidth()) / Program::GetInstance().WindowHeight();
	fov = 70.0f * (float)M_PI / 180.0f;
	fp = 1; bp = 20;
}

mat4 Camera::V()
{
	vec3 w = (wEye - wLookat); w.normalize();
	vec3 u = (cross(wUp, w)); u.normalize();
	vec3 v = cross(w, u); //normalized by default
	mat4 translateToOrigin = TranslateMatrix(wEye * (-1));
	mat4 rotateToAxis = mat4(u.x, v.x, w.x, 0,
							u.y, v.y, w.y, 0,
							u.z, v.z, w.z, 0,
							0, 0, 0, 1);

	return translateToOrigin * rotateToAxis;
}

mat4 Camera::P()
{
	return mat4(1 / (tan(fov / 2) * aspect), 0, 0, 0,
				0, 1 / tan(fov / 2), 0, 0,
				0, 0, -(fp + bp) / (bp - fp), -1,
				0, 0, -2 * fp * bp / (bp - fp), 0);
}

void Camera::Replace(vec3 _wEye, vec3 _wLookat)
{
	wEye = _wEye;
	wLookat = _wLookat;
}

void Camera::ReplaceEye(vec3 _wEye)
{
	wEye = _wEye;
}

void Camera::ReplaceLookat(vec3 _wLookat)
{
	wLookat = _wLookat;
}
