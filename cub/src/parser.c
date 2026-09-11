#include "cub3d.h"

static int	is_texture_identifier(const char *line)
{
	return (line[0] == 'N' && line[1] == 'O' && is_space_char(line[2]))
		|| (line[0] == 'S' && line[1] == 'O' && is_space_char(line[2]))
		|| (line[0] == 'W' && line[1] == 'E' && is_space_char(line[2]))
		|| (line[0] == 'E' && line[1] == 'A' && is_space_char(line[2]));
}

static int	is_color_identifier(const char *line)
{
	return ((line[0] == 'F' || line[0] == 'C') && is_space_char(line[1]));
}

static int	set_texture_path(char **target, char *line)
{
	char	*value;

	if (*target)
		return (0);
	value = trim_left(line);
	if (*value == '\0')
		return (0);
	*target = string_duplicate(value);
	return (*target != NULL);
}

static int	parse_config_line(t_app *app, char *line)
{
	char	*value;

	if (line[0] == 'N' && line[1] == 'O')
		return (set_texture_path(&app->config.north, line + 2));
	if (line[0] == 'S' && line[1] == 'O')
		return (set_texture_path(&app->config.south, line + 2));
	if (line[0] == 'W' && line[1] == 'E')
		return (set_texture_path(&app->config.west, line + 2));
	if (line[0] == 'E' && line[1] == 'A')
		return (set_texture_path(&app->config.east, line + 2));
	value = trim_left(line + 1);
	if (line[0] == 'F')
	{
		if (app->config.has_floor || !parse_color_value(value, &app->config.floor_color))
			return (0);
		app->config.has_floor = 1;
		return (1);
	}
	if (line[0] == 'C')
	{
		if (app->config.has_ceiling || !parse_color_value(value,
			&app->config.ceiling_color))
			return (0);
		app->config.has_ceiling = 1;
		return (1);
	}
	return (0);
}

static int	is_map_character_line(const char *line)
{
	int	has_map_character;

	has_map_character = 0;
	while (*line)
	{
		if (*line == '0' || *line == '1' || *line == 'N' || *line == 'S'
			|| *line == 'E' || *line == 'W')
			has_map_character = 1;
		else if (*line != ' ' && *line != '\t')
			return (0);
		line++;
	}
	return (has_map_character);
}

int	parse_cub_file(t_app *app, const char *path)
{
	char	*content;
	char	*cursor;
	char	*line_start;
	char	*trimmed;
	int		map_started;

	content = read_entire_file(path);
	if (!content)
		return (fatal_error(app, "failed to read file"), 0);
	cursor = content;
	map_started = 0;
	while (*cursor)
	{
		line_start = cursor;
		while (*cursor && *cursor != '\n')
			cursor++;
		if (*cursor == '\n')
			*cursor++ = '\0';
		trim_right_in_place(line_start);
		trimmed = trim_left(line_start);
		if (*trimmed == '\0')
		{
			if (map_started)
				return (free(content), fatal_error(app, "empty line inside map"), 0);
			continue ;
		}
		if (!map_started && (is_texture_identifier(trimmed)
			|| is_color_identifier(trimmed)))
		{
			if (!parse_config_line(app, trimmed))
				return (free(content), fatal_error(app, "invalid configuration"), 0);
		}
		else if (!map_started && is_map_character_line(trimmed))
		{
			map_started = 1;
			if (!parse_map_line(app, line_start))
				return (free(content), 0);
		}
		else if (map_started && is_map_character_line(trimmed))
		{
			if (!parse_map_line(app, line_start))
				return (free(content), 0);
		}
		else
			return (free(content), fatal_error(app, "invalid line in file"), 0);
	}
	free(content);
	return (validate_parsed_map(app));
}