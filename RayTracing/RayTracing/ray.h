#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray {
public:
	ray(){}
	ray(const vec3& a, const vec3& b) { A = a; B = b; }
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t)const { return A + t*B; }

	vec3 A;
	vec3 B;
};

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

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1 - dt*dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else return false;
}


#endif // !RAYH
