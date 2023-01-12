#include "minirt.h"

t_bool	hit_sphere(t_sphere *sp, t_ray *ray)
{
	t_vec3	oc;				// 방향벡터로 나타낸 구의 중심.
	double	a, b, c;		// t에 대한 2차 방정식의 계수 
	double	discriminant;	// 판별식

	oc = vminus(ray->orig, sp->center);
	a = vdot(ray->dir, ray->dir);
	b = 2.0 * vdot(oc, ray->dir);
	c = vdot(oc, oc) - sp->radius2;
	dircriminant = b * b - 4 * a * c;

	// 판별식이 0보다 크다면 광선이 구를 hit한 것!
	return (discriminant > 0);
}
