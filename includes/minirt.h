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

typedef int				t_bool;
# define FALSE	0
# define TRUE	1


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
	// -- image -- 
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	// -- image -- 
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
};

typedef struct	s_mlx_data
{
	void		*mlx;
	void		*win;
	t_canvas	canv;
	t_camera	cam;
	t_ray		ray;
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
t_color3	ray_color(t_ray *ray, t_sphere *sphere); // 광선이 최정적으로 얻게된 픽셀의 색상 값을 리턴
// srcs/hit_sphere.c	(src/trace/hit/hit_sphere.c)
t_bool		hit_sphere(t_sphere *sp, t_ray *ray, t_hit_record *rec);
// srcs/normal.c	(src/trace/hit/normal.c)
void		set_face_normal(t_ray *r, t_hit_record *rec);

// 오브젝트를 만드는 함수는 장면을 구성할 함수이므로 scene.h에 
// srcs/object_create.c	(srcs/scene/object_create.c)
t_sphere	sphere(t_point3 center, double radius);
// srcs/canvas.c	(srcs/scene/canvas.c) (scene.h)
t_canvas	canvas(int width, int height);
// sccs/scene.c		(srcs/scene/scene.c) (scene.h)
t_camera	camera(t_canvas *canvas, t_point3 origin);



#endif
