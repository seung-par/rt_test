#ifndef MINIRT_H
# define MINIRT_H


# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"

# define X_EVENT_KEY_PRESS	2
# define X_EVENT_KEY_EXIT	17
# define X_KEY_ESC			53

typedef struct s_vec3 t_vec3;
typedef struct s_vec3 t_point3;
typedef struct s_vec3 t_color3;

typedef struct s_ray t_ray;

typedef struct s_camera t_camera;
typedef struct s_canvas t_canvas;

typedef struct s_sphere t_sphere;

typedef struct s_hit_record t_hit_record;

// 여러가지의 object를 저장할 수 있어야하고 그 object들을 한번에 가지고 다닐 수 있어야한다.
// ray는 여러개의 object 중 가장 가까이 있는 object에ㅔ 히트를 해야한다. 
// 우선 object라는 리스트를 만들어 각 도형의 정보를 저장할 것이다.
// 그리고 기존에 sphere에만 적용 가능했던 함수들을 조금씩 수정할 예정이다.
typedef struct s_object t_object;

typedef struct s_light t_light;

typedef struct s_scene t_scene;

typedef int				t_bool;
# define FALSE	0
# define TRUE	1

typedef int				t_object_type;
# define SP		0
// 점 광원의 오브젝트 타입 식별자 매크로. (광원도 오브젝트)
# define LIGHT_POINT 1

# define EPSILON 1e-6	// 0.000001

struct	s_vec3
{
	double	x;
	double	y;
	double	z;
};


struct	s_ray
{
	t_point3	orig;
	t_vec3		dir;
};

// 구에 대한 데이터를 저장할 구조체
struct	s_sphere
{
	t_point3	center;
	double		radius;
	double		radius2;
};

// 오브젝트 구조체
struct	s_object
{
	t_object_type	type;
	void			*element;	// 무엇이든 받을 수 있는 void *
	void			*next;
	t_color3		albedo;
};

struct	s_camera
{
	t_point3	orig;			// 카메라 원점(위치)
	double		viewport_h;		// 뷰포트 세로길이 
	double		viewport_w;		// 뷰포트 가로길이
	t_vec3		horizontal;		// 수평길이 벡터
	t_vec3		vertical;		// 수직길이	벡터
	double		focal_len;		// focal length
	t_point3	left_bottom;	// 왼쪽 아래 코너점 
};

struct	s_canvas
{
	int		width;			// canvas width
	int		height;			// canvas height
	double	aspect_ratio;	// 종횡비
};

/*
 * 광선이 오브젝트를 hit했을 때의 정보(교점의 위치와 albedo, 교점에서의 법선 등)를
 * 저장할 구조체.
 *
 * P(t) = O + t * D
 * t는 광선 벡터의 크기. O는 광선의 출발점, 즉 카메라의 위치. 
 * t가 0보다 작다는 것은 방향이 음수이고, 광선이 뒤를 향하고 있다는 것이다.
 * 우리는 카메라의 뒤에 있는 오브젝트는 고려할 필요가 없다. 
 * tmin과 tmax의 값은 각각 0과 INFINITY로 오브젝트가 카메라의 뒤에 있거나 (t < 0),
 * 오브젝트가 카메라로부터 너무 멀 경우를 고려해주기 위함이다.
 */
/*
 * 4단원에서는 hit_sphere 함수는 광선이 구를 hit했는지 아닌지를 확인할 수 있는 값을 리턴했고,
 * 5단원에서는 primary ray의 origin과 교점 사이의 거리에 해당하는 값을 return했다. (*******)
 * 하지만 hit했는지 아닌지 여부만으로는 충분하지 않다. origin과 교점 사이의 거리만으로도 부족하다.
 * 광선을 추적하고 이미지를 렌더링하기 위해서는 더 많은 정보가 필요하다. 
 * 우선 교점의 [정확한 위치]! 가 필요하다. 교점의 위치를 알아야 교점에서의 법선 벡터를 알 수 있고,
 * [법선 벡터를 알아야 광원이 오브젝트에 미치는 영향을 계산할 수 있다.]!!!
 * 그러므로 이러한 정보들을 저장할 구조체가 필요하다.
 */
struct	s_hit_record
{
	t_point3	p;			// 교점의 좌표
	t_vec3		normal;		// 교점에서의 법선
	double		tmin;		
	double		tmax;
	double		t;			// 광선의 원점과 교점 사이의 거리!
	t_bool		front_face;
	// 교점에서의 색깔 등 추가적으로 필요한 정보는 나중에 추가~
	t_color3	albedo;
};

/*
 * 우리는 레이트레이싱이 아닌 퐁 조명 모델을 사용.
 * 퐁 조명 모델은 보다 간단한 물리 법칙에 기반한다. 퐁 조명 모델도 광선을 추적한다.
 * 하지만 딱 한 단계, 광원에서 빛이 나와서 물체에 반사되어 눈에 들어오는 단계만 수학적으로 계산한다.
 * 퐁 조명 모델에 사용되는 중요한 물리학적 기법은 퐁 반사 모델이다. 퐁 반사 모델은 광원에서 나온 
 * 빛이 물체에 반사되어 나올 때, 그 조도가 어느 정도가 되는지를 구한는 데에 활용되는 모델이다.
 * 
 * 퐁 반사 모델은 조도를 구할 때 세 가지 요소를 고려한다. 
 * ambient lighting(주변 조명), diffusing lighting(확산 조명), specular lighting(반사광)
 * 위 세 가지 요소를 결합.
 * albedo = 반사율 (s_hit_record와 s_object 구조체 두 곳에 있음!!!)
 *
 * 퐁 조명 모델의 핵심은 '빛과 오브젝트의 상호작용을 세 가지 요소로 단순화시켜 각각 계산한 뒤 합한 것'
 * 이 광원으로부터 해당 교점(hit point)에 도달한 빛의 양이라는 것이다.
 * (퐁 조명 모델 요소에는 오브젝트가 발산하는 빛인 Emission(방사광)도 있지만 여기선 다루지 않는다)
 *
 * 교점에 도달한 빛 = Ambient + SUM(Specular + Diffuse)
 * Ambient(주변광/환경광): 다른 물체에 의한 반사광, 대기 중의 산란광등을 단순화 시킨 요소이다. 장면 전체의 밝기에 영향을 주는 요소.
 * Diffuse(산란광/난반사광): 난반사를 통해 우리 눈에 도달하는 빛을 단순화 시킨 요소.
 * Specular(반사광/정반사광): 정반사를 통해 우리 눈에 도달하는 빛을 단순화 시킨 요소.
 *
 * 우리는 교점에 도달한 빛의 양을 계산하기 위해, 장면에 존재하는 모든 광원에 대해 각각의
 * Speecular, Diffuse 합을 구한 뒤 최종적으로 Ambient을 더해야 한다. 그 뒤 빛의 총량과 오브젝트의
 * 반사율을 곱해주어 최종적인 픽셀의 색을 얻어온다. 
 * 이를 적용하여 phong_lighting 함수를 만들어보자.
 *
 * 광원에는 기본적으로 평행광원(Direct lighting ex.태양), 점광원(Point light), 집중광원(Spot light)이 있다.
 *  밑의 구조체는 점광원.
 */
struct	s_light
{
	t_point3	origin;
	t_color3	light_color;
	double		bright_ratio;
};

struct	s_scene
{
	t_canvas		canvas;
	t_camera		camera;
	t_object		*world;	// 물체들만 
	t_object		*light;	// 광선들만 
	t_color3		ambient;
	t_ray			ray;
	t_hit_record	rec;
};

typedef struct	s_mlx_data
{
	void		*mlx;
	void		*win;
	t_scene		*scene;
	// -- image -- 
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	// -- image -- 
}				t_mlx_data;

// srcs/print.c
int			write_color(t_color3 pixel_color, FILE *fp);

// srcs/utils.c
t_vec3      vec3(double x, double y, double z);
t_point3    point3(double x, double y, double z);
t_point3    color3(double r, double g, double b);
void        vset(t_vec3 *vec, double x, double y, double z);
double      vlength2(t_vec3 vec);
double      vlength(t_vec3 vec);
t_vec3      vplus(t_vec3 vec, t_vec3 vec2);
t_vec3      vplus_(t_vec3 vec, double x, double y, double z);
t_vec3      vminus(t_vec3 vec, t_vec3 vec2);
t_vec3      vminus_(t_vec3 vec, double x, double y, double z);
t_vec3      vmult(t_vec3 vec, double t);
t_vec3      vmult_(t_vec3 vec, t_vec3 vec2);
t_vec3      vdivide(t_vec3 vec, double t);
double      vdot(t_vec3 vec, t_vec3 vec2);
t_vec3      vcross(t_vec3 vec, t_vec3 vec2);
t_vec3      vunit(t_vec3 vec);
t_vec3      vmin(t_vec3 vec1, t_vec3 vec2);

// srcs/ray.c	(src/trace/ray.c)
t_ray		ray(t_point3 orig, t_vec3 dir);
t_ray		ray_primary(t_camera *cam, double u, double v); // 가장 처음 카메라에서 출발한 광선
t_point3	ray_at(t_ray *ray, double t);
// 광선이 최정적으로 얻게된 픽셀의 색상 값을 리턴
t_color3	ray_color(t_scene *scene);

// srcs/hit.c	(src/trace/hit/hit.c)
t_bool		hit(t_object *obj, t_ray *ray, t_hit_record *rec);
t_bool		hit_obj(t_object *obj, t_ray *ray, t_hit_record *rec);
// srcs/hit_sphere.c	(src/trace/hit/hit_sphere.c)
//t_bool		hit_sphere(t_sphere *sp, t_ray *ray, t_hit_record *rec);
t_bool			hit_sphere(t_object *world, t_ray *ray, t_hit_record *rec);

// srcs/normal.c	(src/trace/hit/normal.c)
void		set_face_normal(t_ray *r, t_hit_record *rec);

// 오브젝트를 만드는 함수는 장면을 구성할 함수이므로 scene.h에 
// srcs/object_create.c	(srcs/scene/object_create.c)
//t_sphere	sphere(t_point3 center, double radius);
t_sphere	*sphere(t_point3 center, double radius);
t_object	*object(t_object_type type, void *element, t_color3 albedo);
// srcs/canvas.c	(srcs/scene/canvas.c) (scene.h)
t_canvas	canvas(int width, int height);
// sccs/scene.c		(srcs/scene/scene.c) (scene.h)
t_camera	camera(t_canvas *canvas, t_point3 origin);

// srcs/object_utils.c	(src/utils/object_utils.c)
void		oadd(t_object **list, t_object *new);	// 리스트에 추가.
t_object	*olast(t_object *list);	// 리스트의 마지막으로 이동하는 함수.

// 점광원 (Point light)
t_light		*light_point(t_point3 light_origin, t_color3 light_color, double bright_ratio);

// t_hit_record 변수를 초기화 해주는 함수. 
// record_init함수에서 tmin값을 초기화 해줄 때 0이 아닌 EPSILON(0.000001)으로 정의했는데,
// 이는 double 데이터 타입이 근사값이기 때문이다. 
// hit함수에서 t의 값이 0에 가까운 값이 나올 때, tmin이 0이면 비교 시 hit 상황이 아닌 때도 
// hit로 판단하는 오차를 만들어낼 수 있기 때문에 tmin값을 EPSILON으로 설정하여 이러한 상황을 
// 예방할 수 있다.
// src/ray.c	(src/trace/ray/ray.c)
t_hit_record	record_init(void);

/*
 * 우리는 교점에 도달한 빛의 양을 계산하기 위해, 장면에 존재하는 모든 광원에 대해 각각의
 * Speecular, Diffuse 합을 구한 뒤 최종적으로 Ambient을 더해야 한다. 그 뒤 빛의 총량과 오브젝트의
 * 반사율을 곱해주어 최종적인 픽셀의 색을 얻어온다. 
 * 이를 적용하여 phong_lighting 함수를 만들어보자.
 */
// src/phong_lighting.c	(src/trace/ray/phong_lighting.c)
t_color3		phong_lighting(t_scene *scene);

#endif
