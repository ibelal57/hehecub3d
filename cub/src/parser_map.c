#include "cub3d.h"

static int	append_map_row(t_map *map, char *row)
{
	char	**rows;
	int		index;
	int		row_length;

	rows = malloc(sizeof(char *) * (map->height + 2));
	if (!rows)
		return (0);
	index = 0;
	while (index < map->height)
	{
		rows[index] = map->grid[index];
		index++;
	}
	rows[map->height] = string_duplicate(row);
	if (!rows[map->height])
		return (free(rows), 0);
	rows[map->height + 1] = NULL;
	free(map->grid);
	map->grid = rows;
	map->height++;
	row_length = (int)string_length(row);
	if (row_length > map->width)
		map->width = row_length;
	return (1);
}

static int	validate_map_charset(char *line)
{
	while (*line)
	{
		if (*line != '0' && *line != '1' && *line != 'N' && *line != 'S'
			&& *line != 'E' && *line != 'W' && *line != ' ')
			return (0);
		line++;
	}
	return (1);
}

int	parse_map_line(t_app *app, char *line)
{
	if (!validate_map_charset(line))
		return (fatal_error(app, "invalid map character"), 0);
	if (!append_map_row(&app->map, line))
		return (fatal_error(app, "map allocation failed"), 0);
	return (1);
}

static char	**build_padded_grid(t_map *map)
{
	char	**grid;
	int		row_index;
	int		column_index;
	int		line_length;

	grid = malloc(sizeof(char *) * (map->height + 1));
	if (!grid)
		return (NULL);
	row_index = 0;
	while (row_index < map->height)
	{
		grid[row_index] = malloc(map->width + 1);
		if (!grid[row_index])
			return (NULL);
		column_index = 0;
		line_length = (int)string_length(map->grid[row_index]);
		while (column_index < map->width)
		{
			if (column_index < line_length)
				grid[row_index][column_index] = map->grid[row_index][column_index];
			else
				grid[row_index][column_index] = ' ';
			column_index++;
		}
		grid[row_index][column_index] = '\0';
		row_index++;
	}
	grid[row_index] = NULL;
	return (grid);
}

static int	is_open_tile(char tile)
{
	return (tile == '0' || tile == 'N' || tile == 'S'
		|| tile == 'E' || tile == 'W');
}

static int	check_neighbours(t_map *map, int row_index, int column_index)
{
	if (row_index == 0 || column_index == 0
		|| row_index == map->height - 1 || column_index == map->width - 1)
		return (0);
	if (map->grid[row_index - 1][column_index] == ' '
		|| map->grid[row_index + 1][column_index] == ' '
		|| map->grid[row_index][column_index - 1] == ' '
		|| map->grid[row_index][column_index + 1] == ' ')
		return (0);
	return (1);
}

static int	validate_map_closed(t_map *map)
{
	int	row_index;
	int	column_index;

	row_index = 0;
	while (row_index < map->height)
	{
		column_index = 0;
		while (column_index < map->width)
		{
			if (is_open_tile(map->grid[row_index][column_index])
				&& !check_neighbours(map, row_index, column_index))
				return (0);
			column_index++;
		}
		row_index++;
	}
	return (1);
}

static int	set_player_from_map(t_app *app, int row_index, int column_index)
{
	char	cell;

	cell = app->map.grid[row_index][column_index];
	if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
	{
		if (app->player.position_x != 0.0 || app->player.position_y != 0.0)
			return (0);
		app->player.position_x = column_index + 0.5;
		app->player.position_y = row_index + 0.5;
		setup_player_from_char(&app->player, cell);
		app->map.grid[row_index][column_index] = '0';
		return (1);
	}
	return (1);
}

int	validate_parsed_map(t_app *app)
{
	int	row_index;
	int	column_index;
	char	**padded_grid;
	int	players;

	if (!app->config.north || !app->config.south || !app->config.west
		|| !app->config.east || !app->config.has_floor
		|| !app->config.has_ceiling)
		return (fatal_error(app, "missing configuration"), 0);
	if (!app->map.height || !app->map.width)
		return (fatal_error(app, "empty map"), 0);
	padded_grid = build_padded_grid(&app->map);
	if (!padded_grid)
		return (fatal_error(app, "map allocation failed"), 0);
	free_map(&app->map);
	app->map.grid = padded_grid;
	players = 0;
	row_index = 0;
	while (row_index < app->map.height)
	{
		column_index = 0;
		while (column_index < app->map.width)
		{
			if (app->map.grid[row_index][column_index] == 'N'
				|| app->map.grid[row_index][column_index] == 'S'
				|| app->map.grid[row_index][column_index] == 'E'
				|| app->map.grid[row_index][column_index] == 'W')
				players++;
			if (!set_player_from_map(app, row_index, column_index))
				return (fatal_error(app, "multiple players found"), 0);
			column_index++;
		}
		row_index++;
	}
	if (players != 1)
		return (fatal_error(app, "expected one player"), 0);
	if (!validate_map_closed(&app->map))
		return (fatal_error(app, "map is not closed"), 0);
	return (1);
}