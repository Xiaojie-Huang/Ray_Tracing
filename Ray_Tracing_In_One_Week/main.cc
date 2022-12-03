#include "../common/rtweekend.h"
#include "../common/color.h"

#include "hittable_list.h"
#include "sphere.h"
#include "../common/camera.h"

#include <iostream>

#pragma region old hit function(aborted)
// double hit_sphere(const point3& center,double radius,const ray& r)
// {
//     //根据二元一次方程根的情况判断光线是否与球相交
//     vec3 oc = r.origin() - center;
//     auto a = r.direction().length_squared();
//     auto half_b = dot(oc,r.direction());
//     auto c = oc.length_squared() - radius*radius;
//     auto result = half_b*half_b - a*c;

//     //为了得到法线，不仅需要知道是否相交，还需要知道具体交点
//     if(result < 0)
//         return -1.0;   
//     else
//         return (-half_b-sqrt(result))/ a;
// }
#pragma endregion

//根据y坐标混合蓝色和白色
color ray_color(const ray& r,const hittable& world) 
{
#pragma region Chapter5
    // if(hit_sphere(point3(0,0,-1),0.5,r))
    //     return color(1,0,0);

    // auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    // if (t > 0.0) 
    // {
    //     //相交则得到对应法向量并将其标准化映射到rgb
    //     vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
    //     return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    // }
    // //不相交的部分映射到背景色
    // vec3 unit_direction = unit_vector(r.direction());
    // //y原属于-1~1,将其映射到0.0~1.0
    // t = 0.5*(unit_direction.y() + 1.0);
    // return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
#pragma endregion

    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() 
{
    // Image
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            color pixel_color(0,0,0);
            //均值反走样
            for(int s = 0;s < samples_per_pixel;++s)
            {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color,samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}