#include "minirt.h"

// Escape: \033, Color code: [0;31m
# define BLACK  "\033[0;30m"
# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE   "\033[0;34m"
# define PERPLE "\033[0;35m"
# define CYAN   "\033[0;36m"
# define WHITE  "\033[0;37m"
# define RESET  "\033[0m"

// 1920 1080
# define IMG_WIDTH	1920
# define IMG_HEIGHT	1080
//# define IMG_WIDTH		400
//# define IMG_HEIGHT		300

FILE	*ppm_file_init(void)
{
	FILE		*fp = fopen("rgb.ppm", "w");

	if (fp == NULL)
		perror("fp == NULL");
	else
		fprintf(fp, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);
	
	return (fp);
}

//mlx_pixel_put(mlx_ptr, win_ptr, i, j, 0x00FFFFFF);
// 원하는 좌표에 해당하는 주소에 color값을 넣는 함수
void    my_mlx_pixel_put(t_mlx_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

// esc key press event
int	key_hook(int keycode, t_mlx_data *data)
{
	if (keycode == 53)
	{
		mlx_destroy_window(data->mlx, data->win);
		exit(0);
	}
	return (0);
}

t_scene	*scene_init(void)
{
	t_scene		*scene;
	t_object	*world;
	t_object	*lights;
	double		ka;	// 8.4 에서 설명

	if (!(scene = (t_scene *)malloc(sizeof(t_scene))))
		return (NULL);
	scene->canvas = canvas(IMG_WIDTH, IMG_HEIGHT);
	scene->camera = camera(&scene->canvas, point3(0, 0, 0));

	world = object(SP, sphere(point3(-2, 0, -5), 2), color3(0.5, 0, 0));	// world에 구1 추가
	oadd(&world, object(SP, sphere(point3(2, 0, -5), 2), color3(0, 0.5, 0)));	// world에 구2 추가
	//  990 으로 했을 때랑 999로 했을 때랑 그림이 다름 --> 계산 시 오차 때문인듯
	oadd(&world, object(SP, sphere(point3(0, -1000, 0), 990), color3(1, 1, 1)));	// world에 구3 추가
	scene->world = world;
	
	// 																					더미 albedo
	lights = object(LIGHT_POINT, light_point(point3(0, 5, 0), color3(1, 1, 1), 0.5), color3(0, 0, 0));
	scene->light = lights;
	ka = 0.1;	// ka = ambient lighting의 강도 (ambient strength)
	// ambient느느 ambient lighting의 색과 ambient lighting의 강도(ambient strength) 계수인 ka의 곱으로 표현된다. ka값은 장면의 원하는 밝기에 따라 [0 ~ 1] 사이의 값으로 설정하면 된다.
	scene->ambient = vmult(color3(1, 1, 1), ka);
	return (scene);
}

t_scene	*mlx_data_init(t_mlx_data *data)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, IMG_WIDTH, IMG_HEIGHT, "Hello World!!!!");
	data->img = mlx_new_image(data->mlx, IMG_WIDTH, IMG_HEIGHT);	// 이미지 객체 생성
	data->addr = mlx_get_data_addr(data->img, 
			&data->bits_per_pixel, 
			&data->line_length,
			&data->endian);	// 이미지 주소 할당
	return (scene_init());
}

int	main(int argc, char *argv[])
{
	if (argc || argv)
		;
	FILE		*fp;
	int			color;
	double		u;
	double		v;
	t_color3	pixel_color;
	t_mlx_data	mlx_data;


	fp = ppm_file_init();

	mlx_data.scene = mlx_data_init(&mlx_data); 	// scene_init();을 내부에 포함함 (scene = scene_init();)

	for (int j = IMG_HEIGHT - 1; j >= 0; --j)
	{
		for (int i = 0; i < IMG_WIDTH; ++i)
		{
			// u와 v값을 이용해 ray.dir를 결정함 !!!
			// ray_primary
			u = (double)i / (IMG_WIDTH - 1);
			v = (double)j / (IMG_HEIGHT - 1);
			// ray from camera origin to pixel
			// u와 v를 사용해 primary ray를 얻고, 그 광선으로 pixel_color를 구함.
			mlx_data.scene->ray = ray_primary(&(mlx_data.scene->camera), u, v);
			pixel_color = ray_color(mlx_data.scene);

			color = write_color(pixel_color, fp); // out to rgb.ppm
			my_mlx_pixel_put(&mlx_data, i, IMG_HEIGHT - 1 - j, color); // j가 max부터 시작이라 거꾸로
		}
	}
	fclose(fp);

	mlx_put_image_to_window(mlx_data.mlx, mlx_data.win, mlx_data.img, 0, 0);
	mlx_key_hook(mlx_data.win, key_hook, &mlx_data);	// esc key press event
	mlx_loop(mlx_data.mlx);	// loop를 돌면서 event를 기다리고, 생성한 윈도우를 Rendering한다.
	return (0);
}

//printf("x: %f, y: %f, z: %f, color: %d\n", pixel_color.x, pixel_color.y, pixel_color.z, color);
//color = ((int)255 << 16) + ((int)255 << 8) + ((int)255); // all white
