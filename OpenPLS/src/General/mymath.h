#ifndef MYMATH_H
#define MYMATH_H



#include <cmath>
#include <math.h>
#include <iostream>



#define _USE_MATH_DEFINES

////////////////////////////////VEC2/////////////////////////////////////////
struct vec2 {
	float x, y;
	explicit vec2(float _x = 0, float _y = 0):x(_x),y(_y){}

	vec2 operator*(float num) const { return vec2(x * num, y * num); }
	vec2 operator/(float num) const { return vec2(x / num, y / num); }
	vec2 operator-(const vec2& v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }

	inline void Rotate() { float a = y; y = -x; x = a; }
	inline float length() const { return sqrt(x * x + y * y); }
	inline void Normalize() {
		vec2 newVec = vec2(x, y) / length();
		x = newVec.x;
		y = newVec.y;
	}

	
};

inline float dot(const vec2& v1, const vec2& v2) { return (v1.x * v2.x + v1.y * v2.y); }

inline float angle(const vec2& v1, const vec2& v2){return acos(dot(v1,v2)/(v1.length()*v2.length()));}

////////////////////////////////VEC3/////////////////////////////////////////

struct vec3 {
	
	float x, y, z;

	explicit vec3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
	
	
	vec3 operator*(float num) const { return vec3(x * num, y * num, z * num); }
	vec3 operator/(float num) const { return vec3(x / num, y / num, z / num); }
	vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }

	inline float length() const { return sqrtf(x*x+y*y+z*z); }

	inline void normalize() { 
		
		vec3 newVec = vec3(x, y, z) * (1 / length());
		x = newVec.x;
		y = newVec.y;
		z = newVec.z;
	}
	
};
inline std::ostream& operator<<(std::ostream& os, const vec3& v){
	os<<v.x<<" "<<v.y<<" "<<v.z<<std::endl;
	return os;
}

inline vec3 normalize(const vec3& v){ return v / v.length();}

inline float dot(const vec3& v1, const vec3& v2) { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

////////////////////////////////VEC4/////////////////////////////////////////

struct vec4 {
	float x, y, z, w;

	explicit vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 0) : x(_x), y(_y), z(_z),w(_w) {}
	explicit vec4(const vec3& v, float _w = 1) : x(v.x), y(v.y), z(v.z), w(_w) {}

	vec4 operator*(float num) const { return vec4(x * num, y * num, z * num, w * num); }
	vec4 operator/(float num) const { return vec4(x / num, y / num, z / num, w / num); }
	vec4 operator+(const vec4& v) const { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	vec4 operator-(const vec4& v)  const { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	vec4 operator*(const vec4& v) const { return vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	void operator+=(const vec4 right) { x += right.x; y += right.y; z += right.z; w += right.w; }

	float& operator[](int index) { return *(&x + index); }
	float operator[](int index) const { return *(&x + index); }

};

inline float dot(const vec4& v1, const vec4& v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

inline vec4 operator*(float a, const vec4& v) {
	return vec4(v.x * a, v.y * a, v.z * a, v.w * a);
}

////////////////////////////////MAT4/////////////////////////////////////////

struct mat4 { 

	vec4 rows[4];

public:
	//Default: unit matrix
	mat4() {
		rows[0][0] = 1; rows[0][1] = 0; rows[0][2] = 0; rows[0][3] = 0;
		rows[1][0] = 0; rows[1][1] = 1; rows[1][2] = 0; rows[1][3] = 0;
		rows[2][0] = 0; rows[2][1] = 0; rows[2][2] = 1; rows[2][3] = 0;
		rows[3][0] = 0; rows[3][1] = 0; rows[3][2] = 0; rows[3][3] = 1;
	}
	mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		rows[0][0] = m00; rows[0][1] = m01; rows[0][2] = m02; rows[0][3] = m03;
		rows[1][0] = m10; rows[1][1] = m11; rows[1][2] = m12; rows[1][3] = m13;
		rows[2][0] = m20; rows[2][1] = m21; rows[2][2] = m22; rows[2][3] = m23;
		rows[3][0] = m30; rows[3][1] = m31; rows[3][2] = m32; rows[3][3] = m33;
	}


	
	inline void Reset() {
		rows[0][0] = 0; rows[0][1] = 0; rows[0][2] = 0; rows[0][3] = 0;
		rows[1][0] = 0; rows[1][1] = 0; rows[1][2] = 0; rows[1][3] = 0;
		rows[2][0] = 0; rows[2][1] = 0; rows[2][2] = 0; rows[2][3] = 0;
		rows[3][0] = 0; rows[3][1] = 0; rows[3][2] = 0; rows[3][3] = 0;
	}

	inline void Transpon() {
		vec4 r[4];
		r[0] = vec4(rows[0][0], rows[1][0], rows[2][0], rows[3][0]);
		r[1] = vec4(rows[0][1], rows[1][1], rows[2][1], rows[3][1]);
		r[2] = vec4(rows[0][2], rows[1][2], rows[2][2], rows[3][2]);
		r[3] = vec4(rows[0][3], rows[1][3], rows[2][3], rows[3][3]);
		rows[0] = r[0];
		rows[1] = r[1];
		rows[2] = r[2];
		rows[3] = r[3];
	}

	vec4& operator[](int i) { return rows[i]; }
	vec4 operator[](int i) const { return rows[i]; }
	operator float* () const { return (float*)this; }

	mat4(vec4 it, vec4 jt, vec4 kt, vec4 ot) {
		rows[0] = it; rows[1] = jt; rows[2] = kt; rows[3] = ot;
	}
};

inline vec4 operator*(const vec4& v, const mat4& mat) {
	return v[0] * mat[0] + v[1] * mat[1] + v[2] * mat[2] + v[3] * mat[3];
}

inline mat4 operator*(const mat4& left, const mat4& right) {
	mat4 result;
	for (int i = 0; i < 4; i++) result.rows[i] = left.rows[i] * right;
	return result;
}

inline std::ostream& operator<<(std::ostream& os, const mat4& m)
{
	os << m[0][0] << m[1][0] << m[2][0] << m[3][0] << '\n' << m[0][1] << m[1][1] << m[2][1] << m[3][1] << '\n' << m[0][2] << m[1][2] << m[2][2] << m[3][2] << '\n' << m[0][3] << m[1][3] << m[2][3] << m[3][3] << '\n';
	return os;
}

inline mat4 TranslateMatrix(vec3 v)
{
	mat4 m;
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
	return m;
}
inline mat4 ScaleMatrix(vec3 v)
{
	mat4 m;
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
	return m;
}

inline mat4 RotationMatrix(float angle, vec3 w) {
	float c = cosf(angle), s = sinf(angle);
	w.normalize();
	mat4 m;

	m[0][0] = c * (1 - w.x * w.x) + w.x * w.x; m[0][1] = w.x * w.y * (1 - c) - w.z * s; m[0][2] = w.x * w.z * (1 - c) + w.y * s; m[0][3] = 0;
	m[1][0] = w.x * w.y * (1 - c) + w.z * s; m[1][1] = c * (1 - w.y * w.y) + w.y * w.y; m[1][2] = w.y * w.z * (1 - c) - w.x * s; m[1][3] = 0;
	m[2][0] = w.x * w.z * (1 - c) - w.y * s; m[2][1] = w.y * w.z * (1 - c) + w.x * s; m[2][2] = c * (1 - w.z * w.z) + w.z * w.z; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

	m.Transpon(); //translate to row-major

	return m;
}
////////////////////////////////MIX/////////////////////////////////////////

inline void TransformPoint(vec3& v, const mat4& m) {
	vec4 temp = vec4(v);
	temp = temp * m;
	temp = temp / temp.w;
	v = vec3(temp.x,temp.y,temp.z);
}


inline bool FE(float a, float b){
	return fabs(a-b)<0.0001f;
}



#endif
