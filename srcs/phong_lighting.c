#include "minirt.h"

t_color3	phong_lighting(t_scene *scene)
{
	t_color3	light_color;
	t_object	*lights;

	light_color = color3(0, 0, 0);	// 광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
	lights = scene->light;
	//(void )lights;
	while (lights)	// 여러 광원에서 나오는 모든 및에 대해 각각 diffuse, specular값을 모두 구해줘야 한다.
	{
		if (lights->type == LIGHT_POINT)
			light_color = vplus(light_color, point_light_get(scene, lights->element));	// point_light_get( , );
		lights = lights->next;
	}
	light_color = vplus(light_color, scene->ambient);
	return (vmin(vmult_(light_color, scene->rec.albedo), color3(1, 1, 1)));
	// 모든 광원에 의한 빛의 양을 구한 후, 오브젝트의 반사율과 곱해준다. 
	// 그 값이 (1, 1, 1)을 넘으면 (1, 1, 1)을 반환한다. 
}

static t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	return (vminus(v, vmult(n, vdot(v, n) * 2)));
}

// Diffuse lighting은 교점에서 출발하여 광원의 중심으로 향하는 벡터(light_dir)와 교점에서의 법선(normal)의 각도를 통해 계산한다.
// 사이각 세타가 [0 ~ 90]도 일 때, cos(세타)값은 [1 ~ 0]이므로, diffuse의 강도 계수인 kd를 cos(세타)값으로 한다.
// 이렇게 구한 diffuse의 강도와 빛의 양을 곱해주면, diffuse를 통해 교점에 도달한 빛의 양을 계산할 수 있다.
t_color3	point_light_get(t_scene *scene, t_light *light)
{
	// ----- diffuse lighting -----
	t_color3	diffuse;
	t_vec3		light_dir;
	double		kd;	// diffuse의 강도(strength)
	// ----- diffuse lighting -----
	// ----- specular lighting -----
	t_color3	specular;
	t_vec3		view_dir;
	t_vec3		reflect_dir;
	double		spec;
	double		ksn;
	double		ks;
	// ----- specular lighting -----
	double		brightness;
	// ----- shadow -----
	double		light_len;
	t_ray		light_ray;
	// ----- shadow -----


	// ----- diffuse lighting -----
	// 교점에서 출발하여 광원을 향하는 벡터(정규화 됨)
	//light_dir = vunit(vminus(light->origin, scene->rec.p));
	light_dir = vminus(light->origin, scene->rec.p);
	light_len = vlength(light_dir);
	light_ray = ray(vplus(scene->rec.p, vmult(scene->rec.normal, EPSILON)), light_dir);
	if (in_shadow(scene->world, light_ray, light_len))
		return (color3(0, 0, 0));
	light_dir = vunit(light_dir);
	// cos(세타)는 세타값이 90도 일 떄 0이고 세타가 둔각이 되면 음수가 되므로 0.0보다 작은 경우는 0.0으로 대체한다. 
	// (교점에서 출발하여 광원을 향하는 벡터)와 (교점에서의 법선벡터)의 내적값.
	kd = fmax(vdot(scene->rec.normal, light_dir), 0.0);
	diffuse = vmult(light->light_color, kd);
	// ----- diffuse lighting -----
	// ----- specular lighting -----
	// specular = ks * (spec ^ ksn)
	view_dir = vunit(vmult(scene->ray.dir, -1));
	reflect_dir = reflect(vmult(light_dir, -1), scene->rec.normal);
	ksn = 1;	// shininess value
	ks = 1;	// specular strength [0 ~ 1]
	spec = pow(fmax(vdot(view_dir, reflect_dir), 0.0), ksn);
	specular = vmult(vmult(light->light_color, ks), spec);
	// ----- specular lighting -----
	brightness = light->bright_ratio * LUMEN;	// 기준 광속/광량을 정의한 매크로 
	return (vmult(vplus(vplus(scene->ambient, diffuse), specular), brightness));
}

//
t_bool	in_shadow(t_object *objs, t_ray light_ray, double light_len)
{
	t_hit_record	rec;

	rec.tmin = 0;
	rec.tmax = light_len;
	if (hit(objs, &light_ray, &rec))
		return (TRUE);
	return (FALSE);
}
