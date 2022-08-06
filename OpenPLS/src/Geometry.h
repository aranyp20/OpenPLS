#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "mymath.h"

struct GRay{
    vec3 START, DIR;
};


struct Triangle{
    vec2 p1;
    vec2 p2;
    vec2 p3;

    Triangle(vec2,vec2,vec2);

    bool Contains(const vec2&);
};

struct Rect{
    vec2 p1;
    vec2 p2;
    float width;

    Rect(vec2,vec2,float);

    vec2 Norm();
    bool Contains(const vec2&);
};


//Untested class!!!
struct Line2D{

    vec2 p1;
    vec2 p2;

    Line2D(vec2,vec2);


    vec2 Norm() const;

    vec2 Intersect(const Line2D&);


    float DistanceFromSection(const vec2&, bool endToo);
    float DistanceFromLine(const vec2&);
    bool SectionContains(const vec2&);
};

//Untested class!!!
struct Cylinder{
    vec3 p1;
    vec3 p2;
    float radius;

    Cylinder(vec3,vec3,float);


    vec3 Intersect(const GRay&) const;

    vec3 IntersectSection(const GRay&) const;

};



#endif 