#include "minirt.h"

// ray 생성자(정규화 된 ray)
t_ray	ray(t_point3 orig, t_vec3 dir)
{
	t_ray	ray;

	ray.orig = orig;
	ray.dir = vunit(dir);
	return (ray);
}

// ***
// ray origin point 부터 방향벡터 ray dir * t 만큼 떨어진 점!
t_point3	ray_at(t_ray *ray, double t)
{
	t_point3	at;

	at = vplus(ray->orig, vmult(ray->dir, t));
	return (at);
}

/*
 * 광선이 최정적으로 얻게된 픽셀의 색상 값을 리턴.
 *
 * n은 정규화된 법선이므로 x, y, z 값이 모두 [-1, 1]의 범위 안에 있다.
 * 우리가 구하고자 하는 범위는 [0, 1]이므로 각각의 값에 1을 더해 범위를 [0, 2]로 변환한 뒤
 * 2로 나누어주어 범위를 [0, 1]로 매핑했다.
 */
//t_color3	ray_color(t_ray *ray, t_sphere *sphere)
t_color3	ray_color(t_ray *ray, t_object *world)
{
	double			t = 0.0;
	t_vec3			n;	// 정규화된 법선 벡터  (P - C) / r
	t_hit_record	rec;

	rec.tmin = 0;
	rec.tmax = INFINITY;
	// 광선이 구에 적중하면 (광선과 구가 교점이 있고, 교점이 카메라 앞쪽이라면!)
	//if (hit_sphere(sphere, ray, &rec))
	if (hit(world, ray, &rec))
		return (vmult(vplus(rec.normal, color3(1, 1, 1)), 0.5));
	if (t > 0.0)
	{
		// 정규화된 구 표면에서의 법선
		//n = vunit(vminus(ray_at(ray, t), sphere->center)); // 임시로 주석
		return (vmult(color3(n.x + 1, n.y + 1, n.z + 1), 0.5));
		//return (color3(1, 0, 0));	// 빨간색(1, 0, 0)
	}
	else
	{
		// ray의 방향벡터의 y값을 기준으로 그라데이션을 주기 위한 계수.
		t = 0.5 * (ray->dir.y + 1.0);
		// (1 - t) * 흰색 + t * 하늘색 
		return (vplus(vmult(color3(1, 1, 1), 1.0 - t), vmult(color3(0.5, 0.7, 1.0), t)));
		// 구를 hit한 경우에는 빨간색을(albedo; 1, 0, 0), 
		// hit하지 못한 경우에는 배경인 하늘색을 표현하도록 수정했다.
	}
}

t_ray	ray_primary(t_camera *cam, double u, double v)
{
	t_ray	ray;

	ray.orig = cam->orig;
	ray.dir = vunit(vminus(vplus(vplus(cam->left_bottom, vmult(cam->horizontal, u)),
					vmult(cam->vertical, v)), cam->orig));
	return (ray);
}

// 광선이 최종적으로 얻게된 픽셀의 생삭 값을 리턴.
/*
t_color3	ray_color(t_ray *r)
{
	double	t;

	t = 0.5 * (r->dir.y + 1.0);
	// (1 - t) * 흰색 + t * 하늘색 
	return (vplus(vmult(color3(1, 1, 1), 1.0 - t), vmult(color3(0.5, 0.7, 1.0), t)));
}
*/
