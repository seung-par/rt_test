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

# define IMG_WIDTH	1920
# define IMG_HEIGHT	1080

// 원하는 좌표에 해당하는 주소에 color값을 넣는 함수
void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

// esc key press event
int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return (0);
}


int	main(int argc, char *argv[])
{
	if (argc || argv)
		;
	int		color;
	t_vars	vars;
	t_image	image;


	t_canvas	canv;
	t_camera	cam;
	t_ray		ray;
	t_sphere	sp;

	t_color3	pixel_color;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, IMG_WIDTH, IMG_HEIGHT, "Hello World!!!!");
	image.img = mlx_new_image(vars.mlx, IMG_WIDTH, IMG_HEIGHT);	// 이미지 객체 생성
	image.addr = mlx_get_data_addr(image.img, 
			&image.bits_per_pixel, 
			&image.line_length,
			&image.endian);	// 이미지 주소 할당

	/*
	canv = canvas(400, 300);
	cam = camera(&canv, point3(0, 0, 0));
	sp = sphere(point3(0, 0, -5), 2);

		// ray from camera origin to pixel
		ray = ray_primary(&cam, u, v);
		pixel_color = ray_color(&ray, &sp);
		write_color(pixel_colr);
	*/

	for (int i = 0; i < IMG_HEIGHT- 1; ++i)
	{
		for (int j = 0; j < IMG_WIDTH - 1; ++j)
		{
			//mlx_pixel_put(mlx_ptr, win_ptr, i, j, 0x00FFFFFF);
			double	r = (double)i / (IMAGE_WIDTH - 1);
			double	g = (double)j / (IMAGE_HEIGHT - 1);
			double	b = 0.25;
			// write_color(pixel_color);
			color = ((int)(255.999 * r) << 16) + ((int)(255.999 * g) << 8) + ((int)(255.999 * b));
			my_mlx_pixel_put(&image, j, i, color);
		}
	}
	
	mlx_put_image_to_window(vars.mlx, vars.win, image.img, 0, 0);
	mlx_key_hook(vars.win, key_hook, &vars);	// esc key press event
	print_image_data(&image);	// debug
	mlx_loop(vars.mlx);	// loop를 돌면서 event를 기다리고, 생성한 윈도우를 Rendering한다.
	return (0);
}
