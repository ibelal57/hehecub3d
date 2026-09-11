#include "cub3d.h"

static int	is_wall_tile(t_app *app, double next_x, double next_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)next_x;
	map_y = (int)next_y;
	if (map_x < 0 || map_y < 0 || map_y >= app->map.height
		|| map_x >= app->map.width)
		return (1);
	return (app->map.grid[map_y][map_x] == '1');
}

static int	is_wall(t_app *app, double next_x, double next_y)
{
	const double	radius = 0.18;

	if (is_wall_tile(app, next_x - radius, next_y - radius))
		return (1);
	if (is_wall_tile(app, next_x + radius, next_y - radius))
		return (1);
	if (is_wall_tile(app, next_x - radius, next_y + radius))
		return (1);
	if (is_wall_tile(app, next_x + radius, next_y + radius))
		return (1);
	return (0);
}

static void	rotate_player(t_player *player, double angle)
{
	double	old_direction_x;
	double	old_plane_x;

	old_direction_x = player->direction_x;
	player->direction_x = player->direction_x * cos(angle)
		- player->direction_y * sin(angle);
	player->direction_y = old_direction_x * sin(angle)
		+ player->direction_y * cos(angle);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(angle)
		- player->plane_y * sin(angle);
	player->plane_y = old_plane_x * sin(angle)
		+ player->plane_y * cos(angle);
}

static void	move_player(t_app *app)
{
	double	move_speed;
	double	next_x;
	double	next_y;

	move_speed = MOVE_SPEED;
	if (app->input.forward)
	{
		next_x = app->player.position_x + app->player.direction_x * move_speed;
		next_y = app->player.position_y + app->player.direction_y * move_speed;
		if (!is_wall(app, next_x, app->player.position_y))
			app->player.position_x = next_x;
		if (!is_wall(app, app->player.position_x, next_y))
			app->player.position_y = next_y;
	}
	if (app->input.backward)
	{
		next_x = app->player.position_x - app->player.direction_x * move_speed;
		next_y = app->player.position_y - app->player.direction_y * move_speed;
		if (!is_wall(app, next_x, app->player.position_y))
			app->player.position_x = next_x;
		if (!is_wall(app, app->player.position_x, next_y))
			app->player.position_y = next_y;
	}
	if (app->input.left)
	{
		next_x = app->player.position_x - app->player.plane_x * move_speed;
		next_y = app->player.position_y - app->player.plane_y * move_speed;
		if (!is_wall(app, next_x, app->player.position_y))
			app->player.position_x = next_x;
		if (!is_wall(app, app->player.position_x, next_y))
			app->player.position_y = next_y;
	}
	if (app->input.right)
	{
		next_x = app->player.position_x + app->player.plane_x * move_speed;
		next_y = app->player.position_y + app->player.plane_y * move_speed;
		if (!is_wall(app, next_x, app->player.position_y))
			app->player.position_x = next_x;
		if (!is_wall(app, app->player.position_x, next_y))
			app->player.position_y = next_y;
	}
	if (app->input.turn_left)
		rotate_player(&app->player, -ROT_SPEED);
	if (app->input.turn_right)
		rotate_player(&app->player, ROT_SPEED);
}

int	game_loop(t_app *app)
{
	if (!app->running)
		return (0);
	handle_mouse(app);
	move_player(app);
	render_scene(app);
	return (0);
}