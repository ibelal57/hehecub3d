#include "cub3d.h"

static int	make_color(t_color color)
{
	return ((color.red << 16) | (color.green << 8) | color.blue);
}

void	draw_pixel(t_image *image, int x, int y, int color)
{
	char	*pixel;

	if (!image || !image->pixels || x < 0 || y < 0
		|| x >= image->width || y >= image->height)
		return ;
	pixel = image->pixels + y * image->line_length + x * (image->bits_per_pixel / 8);
	*(unsigned int *)pixel = (unsigned int)color;
}

int	get_pixel_color(t_image *image, int x, int y)
{
	char	*pixel;

	if (!image || !image->pixels || x < 0 || y < 0
		|| x >= image->width || y >= image->height)
		return (0);
	pixel = image->pixels + y * image->line_length + x * (image->bits_per_pixel / 8);
	return (*(unsigned int *)pixel);
}

void	fill_image(t_image *image, int color)
{
	int	row_index;
	int	column_index;

	row_index = 0;
	while (row_index < image->height)
	{
		column_index = 0;
		while (column_index < image->width)
		{
			draw_pixel(image, column_index, row_index, color);
			column_index++;
		}
		row_index++;
	}
}

static t_texture	*pick_texture(t_app *app, t_ray *ray)
{
	if (ray->side == 0 && ray->ray_dir_x > 0)
		return (&app->textures[TEXTURE_WEST]);
	if (ray->side == 0 && ray->ray_dir_x < 0)
		return (&app->textures[TEXTURE_EAST]);
	if (ray->side == 1 && ray->ray_dir_y > 0)
		return (&app->textures[TEXTURE_NORTH]);
	return (&app->textures[TEXTURE_SOUTH]);
}

static void	draw_vertical_slice(t_app *app, int screen_x, t_ray *ray)
{
	t_texture	*texture;
	double		step;
	double		tex_position;
	int			tex_y;
	int			color;

	texture = pick_texture(app, ray);
	if (ray->side == 0)
		ray->wall_x = app->player.position_y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		ray->wall_x = app->player.position_x + ray->perp_wall_dist * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * (double)texture->image.width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		ray->tex_x = texture->image.width - ray->tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		ray->tex_x = texture->image.width - ray->tex_x - 1;
	step = 1.0 * texture->image.height / ray->line_height;
	tex_position = (ray->draw_start - WIN_HEIGHT / 2 + ray->line_height / 2) * step;
	while (ray->draw_start <= ray->draw_end)
	{
		tex_y = (int)tex_position % texture->image.height;
		if (tex_y < 0)
			tex_y += texture->image.height;
		tex_position += step;
		color = get_pixel_color(&texture->image, ray->tex_x, tex_y);
		draw_pixel(&app->frame, screen_x, ray->draw_start, color);
		ray->draw_start++;
	}
}

static void	cast_ray(t_app *app, int screen_x)
{
	t_ray	ray;
	int		hit;

	ray.camera_x = 2.0 * screen_x / (double)WIN_WIDTH - 1.0;
	ray.ray_dir_x = app->player.direction_x + app->player.plane_x * ray.camera_x;
	ray.ray_dir_y = app->player.direction_y + app->player.plane_y * ray.camera_x;
	ray.map_x = (int)app->player.position_x;
	ray.map_y = (int)app->player.position_y;
	ray.delta_dist_x = fabs(1.0 / ray.ray_dir_x);
	ray.delta_dist_y = fabs(1.0 / ray.ray_dir_y);
	if (ray.ray_dir_x < 0)
	{
		ray.step_x = -1;
		ray.side_dist_x = (app->player.position_x - ray.map_x) * ray.delta_dist_x;
	}
	else
	{
		ray.step_x = 1;
		ray.side_dist_x = (ray.map_x + 1.0 - app->player.position_x) * ray.delta_dist_x;
	}
	if (ray.ray_dir_y < 0)
	{
		ray.step_y = -1;
		ray.side_dist_y = (app->player.position_y - ray.map_y) * ray.delta_dist_y;
	}
	else
	{
		ray.step_y = 1;
		ray.side_dist_y = (ray.map_y + 1.0 - app->player.position_y) * ray.delta_dist_y;
	}
	hit = 0;
	while (!hit)
	{
		if (ray.side_dist_x < ray.side_dist_y)
		{
			ray.side_dist_x += ray.delta_dist_x;
			ray.map_x += ray.step_x;
			ray.side = 0;
		}
		else
		{
			ray.side_dist_y += ray.delta_dist_y;
			ray.map_y += ray.step_y;
			ray.side = 1;
		}
		if (ray.map_x < 0 || ray.map_y < 0 || ray.map_y >= app->map.height
			|| ray.map_x >= app->map.width
			|| app->map.grid[ray.map_y][ray.map_x] == '1')
			hit = 1;
	}
	if (ray.side == 0)
		ray.perp_wall_dist = (ray.map_x - app->player.position_x
			+ (1 - ray.step_x) / 2.0) / ray.ray_dir_x;
	else
		ray.perp_wall_dist = (ray.map_y - app->player.position_y
			+ (1 - ray.step_y) / 2.0) / ray.ray_dir_y;
	if (ray.perp_wall_dist < 0.0001)
		ray.perp_wall_dist = 0.0001;
	ray.line_height = (int)(WIN_HEIGHT / ray.perp_wall_dist);
	ray.draw_start = -ray.line_height / 2 + WIN_HEIGHT / 2;
	if (ray.draw_start < 0)
		ray.draw_start = 0;
	ray.draw_end = ray.line_height / 2 + WIN_HEIGHT / 2;
	if (ray.draw_end >= WIN_HEIGHT)
		ray.draw_end = WIN_HEIGHT - 1;
	draw_vertical_slice(app, screen_x, &ray);
}

void	render_scene(t_app *app)
{
	int	index;
	int	column;

	fill_image(&app->frame, make_color(app->config.ceiling_color));
	index = WIN_HEIGHT / 2;
	while (index < WIN_HEIGHT)
	{
		column = 0;
		while (column < WIN_WIDTH)
		{
			draw_pixel(&app->frame, column, index,
				make_color(app->config.floor_color));
			column++;
		}
		index++;
	}
	index = 0;
	while (index < WIN_WIDTH)
	{
		cast_ray(app, index);
		index++;
	}
	mlx_put_image_to_window(app->mlx, app->window, app->frame.handle, 0, 0);
}