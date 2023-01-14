#include "minirt.h"

// object 함수는 리스트에 추가할 object를 만드는 생성자.
// sphere 함수로 만든 구조체가 object의 element에 들어감.
t_object	*object(t_object_type type, void *element)
{
	t_object	*new;

	if (!(new = (t_object *)malloc(sizeof(t_object))))
		return (NULL);
	new->type = type;
	new->element = element;
	new->next = NULL;
	return (new);
}

// sphere 초기화 함수
//t_sphere	sphere(t_point3 center, double radius)
t_sphere	*sphere(t_point3 center, double radius)
{
	t_sphere	*sp;

	if (!(sp = (t_sphere *)malloc(sizeof(t_sphere))))
		return (NULL);
	sp->center = center;
	sp->radius = radius;
	sp->radius2 = radius * radius;
	return (sp);
}
