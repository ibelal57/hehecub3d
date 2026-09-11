#include "cub3d.h"

static void	setup_default_player(t_app *app)
{
	app->player.direction_x = -1.0;
	app->player.direction_y = 0.0;
	app->player.plane_x = 0.0;
	app->player.plane_y = 0.66;
}

int	main(int argc, char **argv)
{
	t_app	app;

	if (argc != 2)
	{
		write(2, "Error\nUsage: ./cub3d file.cub\n", 31);
		return (1);
	}
	memset(&app, 0, sizeof(app));
	setup_default_player(&app);
	if (!init_app(&app))
		return (1);
	if (!parse_cub_file(&app, argv[1]))
		return (cleanup_app(&app), 1);
	if (!load_textures(&app))
		return (cleanup_app(&app), 1);
	if (!create_frame_image(&app))
		return (cleanup_app(&app), 1);
	app.window = mlx_new_window(app.mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d");
	if (!app.window)
		return (fatal_error(&app, "window creation failed"), 1);
#if defined(CUB3D_USE_MLX) && CUB3D_USE_MLX == 1
# if defined(__linux__)
	mlx_mouse_hide(app.mlx, app.window);
	mlx_mouse_move(app.mlx, app.window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
# else
	mlx_mouse_hide();
	mlx_mouse_move(app.window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
# endif
#else
	mlx_mouse_hide();
	mlx_mouse_move(app.window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
#endif
	app.mouse_x = WIN_WIDTH / 2;
	app.mouse_y = WIN_HEIGHT / 2;
	app.mouse_ready = 1;
	mlx_hook(app.window, 2, 1L << 0, handle_key_press, &app);
	mlx_hook(app.window, 3, 1L << 1, handle_key_release, &app);
	mlx_hook(app.window, 17, 0, handle_close, &app);
	mlx_loop_hook(app.mlx, game_loop, &app);
	mlx_loop(app.mlx);
	cleanup_app(&app);
	return (0);
}