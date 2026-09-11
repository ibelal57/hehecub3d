#ifndef MLX_STUB_H
# define MLX_STUB_H

void	*mlx_init(void);
void	*mlx_new_window(void *mlx_ptr, int width, int height, char *title);
int		mlx_loop(void *mlx_ptr);
int		mlx_loop_hook(void *mlx_ptr, int (*f)(), void *param);
int		mlx_hook(void *win_ptr, int event, int mask, int (*funct)(), void *param);
void	*mlx_new_image(void *mlx_ptr, int width, int height);
char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line,
						int *endian);
int		mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr,
						int x, int y);
int		mlx_destroy_image(void *mlx_ptr, void *img_ptr);
int		mlx_destroy_window(void *mlx_ptr, void *win_ptr);
int		mlx_destroy_display(void *mlx_ptr);
void	*mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width,
						int *height);
int		mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color);
int		mlx_mouse_hide(void);
int		mlx_mouse_show(void);
int		mlx_mouse_move(void *win_ptr, int x, int y);
int		mlx_mouse_get_pos(void *win_ptr, int *x, int *y);

#endif