#include "cub3d.h"

void	cleanup_app(t_app *app)
{
	int	index;

	if (!app)
		return ;
	index = 0;
	while (index < 4)
	{
		free_texture(&app->textures[index], app->mlx);
		index++;
	}
	if (app->frame.handle)
		mlx_destroy_image(app->mlx, app->frame.handle);
	if (app->window)
		mlx_destroy_window(app->mlx, app->window);
	free_config(&app->config);
	free_map(&app->map);
	if (app->mlx)
	{
#if defined(CUB3D_USE_MLX) && CUB3D_USE_MLX == 1
# if defined(__linux__)
		mlx_destroy_display(app->mlx);
# endif
#else
		mlx_destroy_display(app->mlx);
		free(app->mlx);
#endif
	}
}