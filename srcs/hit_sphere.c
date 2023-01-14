#include "minirt.h"

/*
 * -b +- sqrt(b^2 - 4ac)
 *       2a
 * (b * b - 4 * a * c) = discrimiant
 * discriminant > 0 --> 근 2개 
 * discriminant == 0 --> 근 1개
 *
 * C = 구의 중심을 가리키는 벡터 
 * P = 원 위의 한 점을 가리키는 벡터
 * P(t) = O + tD	광선의 방정식 
 * (P - C) * (P - C) = r^2
 * (O + tD - C) * (O - tD - C) = r^2
 * DD*t^2 + 2D(O-C)t + (O-C)(O-C)r^2 = 0
 *     a = D * D
 *     b = 2 * D * (O - C)
 *     c = (O - C) * (O - C) - r*r
 */
/*
 * hit_sphere는 관련 정보를 hit_record에 저장하고 TRUE & FALSE를 반환하므로, 
 * 반환형을 double -> t_bool로 변경
 *
 * half_b는 짝수 근의 공식을 사용하여 계산을 단순화하기 위해 사용.
 */
t_bool	hit_sphere(t_object *sp_obj, t_ray *ray, t_hit_record *rec)
{
	t_vec3	oc;				// 방향벡터로 나타낸 구의 중심.
	double	a, b, c;		// t에 대한 2차 방정식의 계수 
	double	discriminant;	// 판별식
	
	double	sqrtd;
	double	root;
	t_sphere	*sp = (t_sphere *)sp_obj->element;

	oc = vminus(ray->orig, sp->center);
	a = vdot(ray->dir, ray->dir);
	b = 2.0 * vdot(oc, ray->dir);
	c = vdot(oc, oc) - sp->radius2;
	discriminant = b * b - 4 * a * c;

	// 판별식이 0보다 크다면 광선이 구를 hit한 것!
	if (discriminant < 0)
		return (FALSE);	// 실근이 없을 때,
	// 두 실근(t) 중 tmin과 tmax 사이에 있는 근이 있는지 체크 & 작은 근부터 체크.
	sqrtd = sqrt(discriminant);
	root = (-b - sqrtd) / (2 * a);	// 두 근(t) 중 작은 근(t)부터 고려.
	if (root < rec->tmin || rec->tmax < root)
	{
		root = (-b + sqrtd) / (2 * a);	
		// 큰 근(t)조차 tmin보다 작다면 hit하지 않은 것이므로 FALSE 반환!!!!!
		if (root < rec->tmin || rec->tmax < root) 
			return (FALSE);
		/*
		 * 광선이 오브젝트를 관통한다면 두 개의 교점이 생길 것이고, 카메라에는 가까운 교점에 해당하는
		 * 부분만 보일 것이다. 그런데 왜 두 교점을 모두 고려해줬을까? 어떤 구가 카메라를 둘러싸고
		 * 있다고 생각해보자. 그럼  두 근 중 작은 근은 카메라의 뒤쪽에 있는 것이고, 두 근 중 큰 근이
		 * 카메라의 앞에 있게 될 것이다. 위와 같은 경우를 고려해주기 위해 두 근을 모두 확인하는 것이다.
		 */
	}
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->normal = vdivide(vminus(rec->p, sp->center), sp->radius);	// 정규화된 법선 벡터.
	// rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장!!
	// 위에서 언급한, 구가 카메라를 둘러싸고 있는 경우를 고려하기 위함.
	// 카메라가 구의 안쪽에 있다면 광선과 법선은 같은 방향을 향하게 될 것이다.
	// 그러나 오브젝트와 광원 간의 상화작용을 계산하기 위해서는 법선과 광선이 항상 반대방향을
	// 향하고 있어야 한다. 그러므로 법선이 광선의 반대 방향인지 확인하는 함수를 추가했다.
	set_face_normal(ray, rec);
	rec->albedo = sp_obj->albedo;
	return (TRUE);
}
