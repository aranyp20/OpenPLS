#include "Geometry.h"

float SolveQuadratic(float a, float b, float c)
{
    float det = b*b-4*a*c;

    if(det < 0)throw GeometryException("No solution!");

    if(FE(det,0))return  -b / (2*a);


    float r1 = (-b + sqrtf(det))/(2*a);
    float r2 = (-b - sqrtf(det))/(2*a);

    return std::min(r1,r2);

}

Circle::Circle(vec2 _center,float _radius) : center(_center), radius(_radius){}


bool Circle::Contains(const vec2& p)
{
    
    return (p-center).length()<=radius;
}

Line2D::Line2D(vec2 _p1,vec2 _p2) : p1(_p1), p2(_p2) 
{  
}


vec2 Line2D::Norm() const 
{
    vec2 result = p2 - p1;
    result.Rotate();
    result.Normalize();
    return result;
}

vec2 Line2D::Intersect(const Line2D& l)  const
{
    vec2 DIR = p2 - p1;
    vec2 START = p1;

    vec2 normal2 = l.Norm();

    if(FE(dot(normal2,DIR),0))throw GeometryException("Parallel lines!");

    float t1 =-1*dot(normal2,START-l.p2)/dot(normal2,DIR);
    return vec2(START+DIR*t1);
}

float Line2D::DistanceFromLine(const vec2& p) const
{
    vec2 refP = p1;
    if(FE(p1.x,p.x)&&FE(p1.y,p.y))refP = p2;

    return fabs(dot(Norm(),p-refP));
}

bool Line2D::SameSide(const vec2& _p1,const vec2& _p2)
{
    vec2 refP1 = p1;
    if(FE(p1.x,_p1.x)&&FE(p1.y,_p1.y))refP1 = p2;
    vec2 refP2 = p1;
    if(FE(p1.x,_p2.x)&&FE(p1.y,_p2.y))refP2 = p2;

    return (dot(Norm(),_p1-refP1)<0 && dot(Norm(),_p2-refP2)<0) || (dot(Norm(),_p1-refP1)>0 && dot(Norm(),_p2-refP2)>0);
}

bool Line2D::SectionContains(const vec2& p) const
{
    return Rect(p1,p2,0.01f).Contains(p);
}


float Line2D::DistanceFromSection(const vec2& p, bool endToo = true) const
{
    try{
        if(SectionContains(p))return 0;

        if(SectionContains(Intersect(Line2D(p,p+Norm())))){

            return DistanceFromLine(p);
        }else{
            if(!endToo)return -1;
            float d1 = (p-p1).length();
            float d2 = (p-p2).length();
            if(d1<d2)return d1;
            return d2;
        }
    }catch(GeometryException){
        
        return -1;
    }
}

Cylinder::Cylinder(vec3 _p1,vec3 _p2,float _radius) : p1(_p1), p2(_p2), radius(_radius)  
{   
}


vec3 Cylinder::Intersect(const GRay& r) const
{
    vec3 v = p1 - p2; v.normalize();
    vec3 A = r.START - p1 - v * dot(p1,v);
    vec3 B = r.DIR - v * dot(r.DIR,v);
    float a = dot(B,B);
    float b = dot(A,B);
    float c = dot(A,A) - radius * radius;

    try{
        float t = SolveQuadratic(a,b,c);
        return r.DIR * t + r.START;
    }catch(GeometryException){
        throw GeometryException("Missed");
    }

}

//topless cylinder intersection
vec3 Cylinder::IntersectSection(const GRay& r) const
{
    try{
        vec3 pos = Intersect(r);
        if(dot(pos-p1,p2-p1) < 0)throw GeometryException("Hit cylinder, missed section!");
        return pos;

    }catch(GeometryException err){
        throw err;
    }
}


Rect::Rect(vec2 _p1,vec2 _p2,float _height) : p1(_p1), p2(_p2), height(_height)
{
    
}


vec2 Rect::Norm() const
{
    vec2 result = p1-p2;
    result.Rotate();
    result.Normalize();
    return result;
}

bool Rect::Contains(const vec2& p) const
{
    
    std::vector<vec2> tris = GiveCornersTriangle();

    return Triangle(tris[0],tris[1],tris[2]).Contains(p) || Triangle(tris[3],tris[4],tris[5]).Contains(p);
}

Triangle::Triangle(vec2 _p1,vec2 _p2,vec2 _p3) : p1(_p1), p2(_p2), p3(_p3)
{
    
}


bool Triangle::Contains(const vec2& p) const
{

    
    return Line2D(p1,p2).SameSide(p,p3) && Line2D(p2,p3).SameSide(p,p1) && Line2D(p1,p3).SameSide(p,p2);
    

}


GeometryException::GeometryException(std::string msg)
{
    std::cout<<msg<<std::endl;
}

Rect::Rect(float _startX, float _startY, float _width, float _height) : p1(vec2(_startX ,_startY + (_height / 2))), p2(p2 = vec2(_startX+_width,_startY+(_height/2))),height(_height)
{
    startX = _startX;
    startY = _startY;
    width = _width;

}

Rect::Rect(const std::vector<vec2>& v) : Rect(v[0].x,v[0].y,fabs(v[0].x-v[1].x)>fabs(v[0].x-v[2].x)?-(v[0].x-v[1].x) : -(v[0].x-v[2].x),fabs(v[0].y-v[1].y)>fabs(v[0].y-v[2].y)?-(v[0].y-v[1].y):-(v[0].y-v[2].y))
{
    
  

   
}


std::vector<vec2> Rect::GiveCorners() const
{
    std::vector<vec2> result ;
    result.push_back(p1+Norm()*(height/2));
    result.push_back(p2+Norm()*(height/2));
    result.push_back(p2-Norm()*(height/2));
    result.push_back(p1-Norm()*(height/2));
    return result;
}

std::vector<vec2> Rect::GiveCornersTriangle() const
{
    std::vector<vec2> result;
    result.push_back(p1-Norm()*(height/2));
    result.push_back(p2+Norm()*(height/2));
    result.push_back(p1+Norm()*(height/2));
    result.push_back(p2-Norm()*(height/2));
    result.push_back(p1-Norm()*(height/2));
    result.push_back(p2+Norm()*(height/2));
    return result;
}

void Rect::Recalculate()
{
    p1 = vec2(startX ,startY + (height / 2));
    p2 = vec2(startX+width,startY+(height/2));
}

void Rect::SetHeight(float f)
{
    height = f;
    Recalculate();
}

void Rect::SetWidth(float f)
{
    width = f;
    Recalculate();
}

void Rect::SetStartingX(float f)
{
    startX = f;
    Recalculate();
}

void Rect::SetStartingY(float f)
{
    startY = f;
    Recalculate();
}

