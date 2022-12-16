#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__


#include <vector>
#include "mymath.h"

struct GRay{
    vec3 START, DIR;
};


struct Triangle{
    vec2 p1;
    vec2 p2;
    vec2 p3;

    Triangle(vec2,vec2,vec2);

    bool Contains(const vec2&) const;
};

struct Rect{
    vec2 p1;
    vec2 p2;
    float height;

    
    float startX,startY,width;

    Rect(vec2,vec2,float);
    Rect(float _startX = 0, float _startY = 0, float _width = 0, float _height = 0);
    Rect(const std::vector<vec2>&);


    void SetWidth(float);
    void SetHeight(float);
    void SetStartingX(float);
    void SetStartingY(float);

    vec2 Norm() const;
    bool Contains(const vec2&) const;

    std::vector<vec2> GiveCorners() const;
    std::vector<vec2> GiveCornersTriangle() const;


    private:

    void Recalculate();
};

struct Circle{
    vec2 center;
    float radius;

    Circle(vec2,float);

    bool Contains(const vec2&);
};

//Untested class!!!
struct Line2D{

    vec2 p1;
    vec2 p2;

    Line2D(vec2,vec2);


    vec2 Norm() const;

    vec2 Intersect(const Line2D&) const;

    bool SameSide(const vec2&,const vec2&);

    float DistanceFromSection(const vec2&, bool endToo) const;
    float DistanceFromLine(const vec2&) const;
    bool SectionContains(const vec2&) const;
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


class GeometryException : public std::exception{
    public:
    GeometryException(std::string);
};


#endif 