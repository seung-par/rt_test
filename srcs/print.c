#include "minirt.h"

int	write_color(t_color3 pixel_color, FILE *fp)
{
	fprintf(fp, "%d %d %d\n", (int)(255.999 * pixel_color.x),
						(int)(255.999 * pixel_color.y),
						(int)(255.999 * pixel_color.z));

	/*
	int	color = (pixel_color.x << 16) + (pixel_color.y << 8) + (pixel_color.z);
	my_mlx_pixel_put(&image, i, j, color);
	*/
	int	color = ((int)(255.999 * pixel_color.x) << 16)
				+ ((int)(255.999 * pixel_color.y) << 8)
				+ ((int)(255.999) * pixel_color.z);
	return (color);
}
