#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "mlx_compat.h"

# define WIN_WIDTH 1200
# define WIN_HEIGHT 800
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.04
# define MOUSE_SENSITIVITY 0.0025

/*
** Key values below are X11 keysyms (Linux/minilibx-linux), since
** mlx_int_param_KeyPress/KeyRelease pass XkbKeycodeToKeysym() results,
** not raw platform keycodes. They differ from the macOS mlx keycodes.
*/
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_DESTROY 17

typedef struct s_color
{
	int	red;
	int	green;
	int	blue;
} t_color;

typedef struct s_image
{
	void	*handle;
	char	*pixels;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
} t_image;

typedef struct s_texture
{
	t_image	image;
} t_texture;

typedef struct s_player
{
	double	position_x;
	double	position_y;
	double	direction_x;
	double	direction_y;
	double	plane_x;
	double	plane_y;
} t_player;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
} t_map;

typedef struct s_config
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	t_color	floor_color;
	t_color	ceiling_color;
	int		has_floor;
	int		has_ceiling;
} t_config;

typedef struct s_input
{
	int	forward;
	int	backward;
	int	left;
	int	right;
	int	turn_left;
	int	turn_right;
} t_input;

typedef struct s_app
{
	void		*mlx;
	void		*window;
	t_image		frame;
	t_texture	textures[4];
	t_config	config;
	t_map		map;
	t_player	player;
	t_input		input;
	int			mouse_x;
	int			mouse_y;
	int			mouse_ready;
	int			running;
} t_app;

typedef enum e_texture_index
{
	TEXTURE_NORTH,
	TEXTURE_SOUTH,
	TEXTURE_WEST,
	TEXTURE_EAST
} t_texture_index;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tex_x;
} t_ray;

char		*read_entire_file(const char *path);
char		*string_duplicate(const char *string);
char		*string_join(const char *left, const char *right);
char		*string_substr(const char *string, size_t start, size_t len);
size_t		string_length(const char *string);
int			string_compare(const char *left, const char *right);
int			is_space_char(int character);
int			is_digit_char(int character);
char		*trim_left(char *string);
void		trim_right_in_place(char *string);

void		fatal_error(t_app *app, const char *message);
void		free_string_array(char **array);
void		free_config(t_config *config);
void		free_map(t_map *map);
void		free_texture(t_texture *texture, void *mlx);

int			parse_cub_file(t_app *app, const char *path);
int			parse_color_value(const char *line, t_color *color);
int			parse_map_line(t_app *app, char *line);
int			validate_parsed_map(t_app *app);

int			init_app(t_app *app);
int			load_textures(t_app *app);
int			create_frame_image(t_app *app);
void		setup_player_from_char(t_player *player, char orientation);

void		render_scene(t_app *app);
void		draw_pixel(t_image *image, int x, int y, int color);
int			get_pixel_color(t_image *image, int x, int y);
void		fill_image(t_image *image, int color);

int			handle_key_press(int keycode, t_app *app);
int			handle_key_release(int keycode, t_app *app);
int			handle_close(void *param);
void		handle_mouse(t_app *app);
int			game_loop(t_app *app);

void		cleanup_app(t_app *app);

#endif