#include "cub3d.h"

void	fatal_error(t_app *app, const char *message)
{
	write(2, "Error\n", 6);
	if (message)
	{
		write(2, message, string_length(message));
		write(2, "\n", 1);
	}
	cleanup_app(app);
}

void	free_config(t_config *config)
{
	free(config->north);
	free(config->south);
	free(config->west);
	free(config->east);
	config->north = NULL;
	config->south = NULL;
	config->west = NULL;
	config->east = NULL;
}

void	free_map(t_map *map)
{
	int	row_index;

	if (!map || !map->grid)
		return ;
	row_index = 0;
	while (row_index < map->height)
	{
		free(map->grid[row_index]);
		row_index++;
	}
	free(map->grid);
	map->grid = NULL;
}

void	free_texture(t_texture *texture, void *mlx)
{
	if (texture->image.handle)
		mlx_destroy_image(mlx, texture->image.handle);
	texture->image.handle = NULL;
	texture->image.pixels = NULL;
}