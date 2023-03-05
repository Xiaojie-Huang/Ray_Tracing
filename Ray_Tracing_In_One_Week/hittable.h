#ifndef HITTABLE_H
#define HITTABLE_H

#include "../common/rtweekend.h"
#include "../Ray_Tracing_in_Next_Week/aabb.h"

class material;


struct hit_record
{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;

    //for texture
    double u;
    double v;

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
    virtual bool bounding_box(double time0,double time1,aabb& output_box) const = 0;
};


#endif