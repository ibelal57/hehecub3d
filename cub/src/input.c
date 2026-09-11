#include "cub3d.h"

static void	request_exit(t_app *app)
{
	app->running = 0;
	cleanup_app(app);
	exit(0);
}

static void	rotate_from_mouse_delta(t_player *player, int delta_x)
{
	double	angle;
	double	old_direction_x;
	double	old_plane_x;

	angle = (double)delta_x * MOUSE_SENSITIVITY;
	old_direction_x = player->direction_x;
	player->direction_x = player->direction_x * cos(angle)
		+ player->direction_y * sin(angle);
	player->direction_y = -old_direction_x * sin(angle)
		+ player->direction_y * cos(angle);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(angle)
		+ player->plane_y * sin(angle);
	player->plane_y = -old_plane_x * sin(angle)
		+ player->plane_y * cos(angle);
}

int	handle_key_press(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		request_exit(app);
	else if (keycode == KEY_W)
		app->input.forward = 1;
	else if (keycode == KEY_S)
		app->input.backward = 1;
	else if (keycode == KEY_A)
		app->input.left = 1;
	else if (keycode == KEY_D)
		app->input.right = 1;
	else if (keycode == KEY_LEFT)
		app->input.turn_left = 1;
	else if (keycode == KEY_RIGHT)
		app->input.turn_right = 1;
	return (0);
}

int	handle_key_release(int keycode, t_app *app)
{
	if (keycode == KEY_W)
		app->input.forward = 0;
	else if (keycode == KEY_S)
		app->input.backward = 0;
	else if (keycode == KEY_A)
		app->input.left = 0;
	else if (keycode == KEY_D)
		app->input.right = 0;
	else if (keycode == KEY_LEFT)
		app->input.turn_left = 0;
	else if (keycode == KEY_RIGHT)
		app->input.turn_right = 0;
	return (0);
}

int	handle_close(void *param)
{
	t_app	*app;

	app = param;
	request_exit(app);
	return (0);
}

void	handle_mouse(t_app *app)
{
	int	current_x;
	int	current_y;
	int	delta_x;

	if (!app || !app->window)
		return ;
#if defined(CUB3D_USE_MLX) && CUB3D_USE_MLX == 1
# if defined(__linux__)
	mlx_mouse_get_pos(app->mlx, app->window, &current_x, &current_y);
# else
	mlx_mouse_get_pos(app->window, &current_x, &current_y);
# endif
#else
	mlx_mouse_get_pos(app->window, &current_x, &current_y);
#endif
	if (!app->mouse_ready)
	{
		app->mouse_x = current_x;
		app->mouse_y = current_y;
		app->mouse_ready = 1;
		return ;
	}
	delta_x = current_x - app->mouse_x;
	if (delta_x != 0)
		rotate_from_mouse_delta(&app->player, delta_x);
#if defined(CUB3D_USE_MLX) && CUB3D_USE_MLX == 1
# if defined(__linux__)
	mlx_mouse_move(app->mlx, app->window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
# else
	mlx_mouse_move(app->window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
# endif
#else
	mlx_mouse_move(app->window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
#endif
	app->mouse_x = WIN_WIDTH / 2;
	app->mouse_y = WIN_HEIGHT / 2;
}