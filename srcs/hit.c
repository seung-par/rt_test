#include "minirt.h"

// 각 object를 순회하며 ray를 맞았는지 확인하기 위하여 hit라는 함수를 만들어 준다.
// 우리는 여러가지 오브젝트가 존재할 때에 더 가까이 있는 오브젝트를 히트해야한다.
// 그러므로 어떠한 오브젝트에 히트하면 tmax를 히트한 t로 바꾸어 그 다음 오브젝트를 검사할 때에
// 더 멀리 있는 오브젝트일 경우 히트가 안되게 했다. 
t_bool	hit(t_object *world, t_ray *ray, t_hit_record *rec)
{
	t_bool			hit_anything;
	t_hit_record	temp_rec;

	temp_rec = *rec;	// temp_rec의 tmin, tmax 값 초기화를 위해. 
	hit_anything = FALSE;
	while (world)
	{
		if (hit_obj(world, ray, &temp_rec))
		{
			hit_anything = TRUE;
			temp_rec.tmax = temp_rec.t;
			*rec = temp_rec;
		}
		world = world->next;
	}
	return (hit_anything);
}

// hit_obj는 오브젝트 타입에 맞는 hit함수로 연결해주는 관문
// hit_opj에서는 리스트에 어떤 object가 저장되어 있는지 확인한 후 각각의 object에 맞는 함수를
// 실행할 수 있게 분기해주었다.
t_bool	hit_obj(t_object *world, t_ray *ray, t_hit_record *rec)
{
	t_bool	hit_result;

	hit_result = FALSE;
	if (world->type == SP)
		hit_result = hit_sphere(world, ray, rec);	// hit_sphere의 첫번째 인자도 t_sphere *에서 t_object *로 수정해주자.
	return (hit_result);
}
