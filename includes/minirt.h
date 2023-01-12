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

typedef int				t_bool;
# define FALSE	0
# define TRUE	1

typedef struct	s_vars
{
	void	*mlx;
	void	*win;
}				t_vars;

// canvas vs image (?) 하나로 합침₩ 
// 이미지의 정보를 나타내는 변수를 저장한 구조체
typedef struct	s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_image;

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
	
	void	*img;
	char	*addr;

};

// srcs/print.c
void		write_color(t_color3 pixel_color);

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
t_color3	ray_color(t_ray *ray, t_sphere *sphere); // 광선이 최정적으로 얻게된 픽셀의 색상 값을 리턴.
t_color3	ray_color(t_ray *r);

// srcs/object_create.c	(srcs/scene/object_create.c)
t_sphere	sphere(t_point3 center, double radius);

// srcs/canvas.c	(srcs/scene/canvas.c) (scene.h)
t_canvas	canvas(int width, int height);
// sccs/scene.c		(srcs/scene/scene.c) (scene.h)
t_camera	camera(t_canvas *canvas, t_point3 origin);

// srcs/hit_sphere.c	(src/trace/hit/hit_sphere.c)
t_bool		hit_sphere(t_sphere *sp, t_ray *ray);

#endif
