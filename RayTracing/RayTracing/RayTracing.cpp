// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include <limits>
#include <float.h>
#include "camera.h"

float hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;

	float discriminant = b*b - 4*a*c;
	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt(discriminant)) / (2.0*a);
	}
}

float randomZeroToOne()
{
	return (rand() % 100) / float(100);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(randomZeroToOne(), randomZeroToOne(), randomZeroToOne()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	
	return p;
}

vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.001, (std::numeric_limits<float>::max)(), rec)) {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p, target - rec.p), world);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;	// 采样次数
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	
	camera cam;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + randomZeroToOne()) / float(nx);	// 此处写法为了规避rand函数伪随机的问题
				float v = float(j + randomZeroToOne()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

