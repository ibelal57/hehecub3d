#include "cub3d.h"

typedef struct s_stub_image
{
	char	*pixels;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
} t_stub_image;

void	*mlx_init(void)
{
	return (malloc(1));
}

void	*mlx_new_window(void *mlx_ptr, int width, int height, char *title)
{
	(void)mlx_ptr;
	(void)width;
	(void)height;
	(void)title;
	return (malloc(1));
}

int	mlx_loop(void *mlx_ptr)
{
	(void)mlx_ptr;
	return (0);
}

int	mlx_loop_hook(void *mlx_ptr, int (*f)(), void *param)
{
	(void)mlx_ptr;
	(void)f;
	(void)param;
	return (0);
}

int	mlx_hook(void *win_ptr, int event, int mask, int (*funct)(), void *param)
{
	(void)win_ptr;
	(void)event;
	(void)mask;
	(void)funct;
	(void)param;
	return (0);
}

void	*mlx_new_image(void *mlx_ptr, int width, int height)
{
	t_stub_image	*image;

	(void)mlx_ptr;
	image = malloc(sizeof(*image));
	if (!image)
		return (NULL);
	image->pixels = calloc((size_t)width * (size_t)height, 4);
	if (!image->pixels)
		return (free(image), NULL);
	image->width = width;
	image->height = height;
	image->bits_per_pixel = 32;
	image->line_length = width * 4;
	image->endian = 0;
	return (image);
}

char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line,
	int *endian)
{
	t_stub_image	*image;

	image = (t_stub_image *)img_ptr;
	if (!image)
		return (NULL);
	if (bits_per_pixel)
		*bits_per_pixel = image->bits_per_pixel;
	if (size_line)
		*size_line = image->line_length;
	if (endian)
		*endian = image->endian;
	return (image->pixels);
}

int	mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr,
	int x, int y)
{
	(void)mlx_ptr;
	(void)win_ptr;
	(void)img_ptr;
	(void)x;
	(void)y;
	return (0);
}

int	mlx_destroy_image(void *mlx_ptr, void *img_ptr)
{
	t_stub_image	*image;

	(void)mlx_ptr;
	image = (t_stub_image *)img_ptr;
	if (!image)
		return (0);
	free(image->pixels);
	free(image);
	return (0);
}

int	mlx_destroy_window(void *mlx_ptr, void *win_ptr)
{
	(void)mlx_ptr;
	free(win_ptr);
	return (0);
}

int	mlx_destroy_display(void *mlx_ptr)
{
	(void)mlx_ptr;
	return (0);
}

void	*mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width,
	int *height)
{
	t_stub_image	*image;
	int				fd;

	(void)mlx_ptr;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	close(fd);
	image = malloc(sizeof(*image));
	if (!image)
		return (NULL);
	image->width = 64;
	image->height = 64;
	image->bits_per_pixel = 32;
	image->line_length = 64 * 4;
	image->endian = 0;
	image->pixels = calloc(64 * 64, 4);
	if (!image->pixels)
		return (free(image), NULL);
	if (width)
		*width = image->width;
	if (height)
		*height = image->height;
	return (image);
}

int	mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color)
{
	(void)mlx_ptr;
	(void)win_ptr;
	(void)x;
	(void)y;
	(void)color;
	return (0);
}

int	mlx_mouse_hide(void)
{
	return (0);
}

int	mlx_mouse_show(void)
{
	return (0);
}

int	mlx_mouse_move(void *win_ptr, int x, int y)
{
	(void)win_ptr;
	(void)x;
	(void)y;
	return (0);
}

int	mlx_mouse_get_pos(void *win_ptr, int *x, int *y)
{
	(void)win_ptr;
	if (x)
		*x = 0;
	if (y)
		*y = 0;
	return (0);
}