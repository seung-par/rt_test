#include "minirt.h"

t_color3	phong_lighting(t_scene *scene)
{
	t_color3	light_color;
	t_object	*lights;

	light_color = color3(0, 0, 0);	// 광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
	lights = scene->light;
	/* 임시 주석
	while (lights)	// 여러 광원에서 나오는 모든 및에 대해 각각 diffuse, specular값을 모두 구해줘야 한다.
	{
		if (lights->type == LIGHT_POINT)
			light_color = vplus(light_color, point_light_get(scene, lights->element));	// point_light_get( , );
		lights = lights->next;
	}
	*/
	light_color = vplus(light_color, scene->ambient);
	return (vmin(vmult_(light_color, scene->rec.albedo), color3(1, 1, 1)));
	// 모든 광원에 의한 빛의 양을 구한 후, 오브젝트의 반사율과 곱해준다. 
	// 그 값이 (1, 1, 1)을 넘으면 (1, 1, 1)을 반환한다. 
}
