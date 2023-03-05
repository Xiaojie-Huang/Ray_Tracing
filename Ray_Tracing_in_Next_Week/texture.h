#ifndef TEXTURE_H
#define TEXTURE_H

#include "../common/rtweekend.h"

//贴图基类
class texture
{
    public:
        virtual color value(double u,double v,const point3& p) const = 0;
};

//纯色贴图类
class solid_color : public texture
{
    public:
        solid_color(){}

        solid_color(color c) : color_value(c){} 

        solid_color(double red,double green,double blue)
        : solid_color(color(red,green,blue)){}

        virtual color value(double u,double v,const vec3& p) const override
        {
            //纯色类，不进行贴图采样，直接返回
            return color_value;
        }
    private:
        color color_value;
};

class checker_texture : public texture 
{
    public:
        checker_texture() {}

        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(_even), odd(_odd) {}

        checker_texture(color c1, color c2)
            : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)) {}

        virtual color value(double u, double v, const point3& p) const override 
        {
            auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;
};

#endif