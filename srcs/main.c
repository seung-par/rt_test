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

//mlx_pixel_put(mlx_ptr, win_ptr, i, j, 0x00FFFFFF);
// 원하는 좌표에 해당하는 주소에 color값을 넣는 함수
void    my_mlx_pixel_put(t_mlx_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->canv.addr + (y * data->canv.line_length + x * (data->canv.bits_per_pixel / 8));
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

int	mlx_data_init(t_mlx_data *data)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, IMG_WIDTH, IMG_HEIGHT, "Hello World!!!!");
	// 밑에 2줄은 순서 지켜야 됨.
	// img 먼너 넣어주고 canv 생성자 호출하면 img가 다른 값으로 덮어씌워짐...
	data->canv = canvas(IMG_WIDTH, IMG_HEIGHT);
	data->canv.img = mlx_new_image(data->mlx, IMG_WIDTH, IMG_HEIGHT);	// 이미지 객체 생성
	data->canv.addr = mlx_get_data_addr(data->canv.img, 
			&data->canv.bits_per_pixel, 
			&data->canv.line_length,
			&data->canv.endian);	// 이미지 주소 할당

	data->cam = camera(&data->canv, point3(0, 0, 0));

	/*
	 * Scene setting;
	 * canv = canvas(400, 300);
	 * cam = camera(&canv, point3(0, 0, 0));
	 */
	return (0);
}

FILE	*ppm_file_init(void)
{
	FILE		*fp = fopen("rgb.ppm", "w");

	if (fp == NULL)
		perror("fp == NULL");
	else
		fprintf(fp, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);
	
	return (fp);
}

int	main(int argc, char *argv[])
{
	if (argc || argv)
		;
	FILE		*fp;
	int			color;
	double		u;
	double		v;
	t_mlx_data	mlx_data;
	t_color3	pixel_color;

	t_sphere	sp = sphere(point3(0, 0, -5), 2);	// 4

	fp = ppm_file_init();
	mlx_data_init(&mlx_data);

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
			mlx_data.ray = ray_primary(&(mlx_data.cam), u, v);
			//pixel_color = ray_color(&(mlx_data.ray)); -> 4에서 구를 다룰 수 있도록 수정
			pixel_color = ray_color(&(mlx_data.ray), &sp);

			color = write_color(pixel_color, fp); // out to rgb.ppm
			my_mlx_pixel_put(&mlx_data, i, IMG_HEIGHT - 1 - j, color); // j가 max부터 시작이라 거꾸로
		}
	}
	fclose(fp);

	mlx_put_image_to_window(mlx_data.mlx, mlx_data.win, mlx_data.canv.img, 0, 0);
	mlx_key_hook(mlx_data.win, key_hook, &mlx_data);	// esc key press event
	mlx_loop(mlx_data.mlx);	// loop를 돌면서 event를 기다리고, 생성한 윈도우를 Rendering한다.
	return (0);
}

//printf("x: %f, y: %f, z: %f, color: %d\n", pixel_color.x, pixel_color.y, pixel_color.z, color);
//color = ((int)255 << 16) + ((int)255 << 8) + ((int)255); // all white
