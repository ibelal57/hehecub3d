#include "cub3d.h"

int	init_app(t_app *app)
{
	app->mlx = mlx_init();
	if (!app->mlx)
	{
		write(2, "Error\nmlx_init failed\n", 23);
		return (0);
	}
	app->running = 1;
	return (1);
}

int	create_frame_image(t_app *app)
{
	app->frame.handle = mlx_new_image(app->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!app->frame.handle)
		return (0);
	app->frame.pixels = mlx_get_data_addr(app->frame.handle,
			&app->frame.bits_per_pixel, &app->frame.line_length,
			&app->frame.endian);
	app->frame.width = WIN_WIDTH;
	app->frame.height = WIN_HEIGHT;
	return (app->frame.pixels != NULL);
}

static int	load_single_texture(t_app *app, t_texture *texture,
	char *path)
{
	int	width;
	int	height;

	texture->image.handle = mlx_xpm_file_to_image(app->mlx, path, &width,
			&height);
	if (!texture->image.handle)
		return (0);
	texture->image.pixels = mlx_get_data_addr(texture->image.handle,
			&texture->image.bits_per_pixel, &texture->image.line_length,
			&texture->image.endian);
	texture->image.width = width;
	texture->image.height = height;
	return (texture->image.pixels != NULL);
}

int	load_textures(t_app *app)
{
	if (!load_single_texture(app, &app->textures[TEXTURE_NORTH], app->config.north))
		return (fatal_error(app, "failed to load north texture"), 0);
	if (!load_single_texture(app, &app->textures[TEXTURE_SOUTH], app->config.south))
		return (fatal_error(app, "failed to load south texture"), 0);
	if (!load_single_texture(app, &app->textures[TEXTURE_WEST], app->config.west))
		return (fatal_error(app, "failed to load west texture"), 0);
	if (!load_single_texture(app, &app->textures[TEXTURE_EAST], app->config.east))
		return (fatal_error(app, "failed to load east texture"), 0);
	return (1);
}

void	setup_player_from_char(t_player *player, char orientation)
{
	if (orientation == 'N')
	{
		player->direction_x = 0.0;
		player->direction_y = -1.0;
		player->plane_x = 0.66;
		player->plane_y = 0.0;
	}
	else if (orientation == 'S')
	{
		player->direction_x = 0.0;
		player->direction_y = 1.0;
		player->plane_x = -0.66;
		player->plane_y = 0.0;
	}
	else if (orientation == 'E')
	{
		player->direction_x = 1.0;
		player->direction_y = 0.0;
		player->plane_x = 0.0;
		player->plane_y = 0.66;
	}
	else if (orientation == 'W')
	{
		player->direction_x = -1.0;
		player->direction_y = 0.0;
		player->plane_x = 0.0;
		player->plane_y = -0.66;
	}
}