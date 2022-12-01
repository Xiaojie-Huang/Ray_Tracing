#include "../common/color.h"
#include "../common/vec3.h"
#include "../common/ray.h"

#include <iostream>

bool hit_sphere(const point3& center,double radius,const ray& r)
{
    //根据二元一次方程根的情况判断光线是否与球相交
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(),r.direction());
    auto b = 2.0 * dot(oc,r.direction());
    auto c = dot(oc,oc) - radius*radius;
    auto result = b*b - 4*a*c;

    return (result>0);
}

//根据y坐标混合蓝色和白色
color ray_color(const ray& r) 
{
    if(hit_sphere(point3(0,0,-1),0.5,r))
        return color(1,0,0);

    vec3 unit_direction = unit_vector(r.direction());
    //y原属于-1~1,将其映射到0.0~1.0
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() 
{
    // Image
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width,0,0);
    auto vectical = vec3(0,viewport_height,0);
    auto lower_left_corner = origin - horizontal/2 - vectical/2-vec3(0,0,focal_length);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            ray r(origin,lower_left_corner + u*horizontal + v*vectical-origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}