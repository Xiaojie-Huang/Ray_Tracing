#ifndef HITTABLE_H
#define HITTABLE_H

#include "../common/ray.h"

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    //用来判断内外表面，通过光线和法线点积的正负号来判断,这里假定法线均是指向物体外的
    bool front_face;

    inline void set_face_normal(const ray& r,const vec3& outward_normal)
    {
        front_face = dot(r.direction(),outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{    
public:
    virtual bool hit(const ray& r,double t_min,double t_max,hit_record& rec) const = 0;   
};


#endif