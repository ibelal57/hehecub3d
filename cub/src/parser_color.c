#include "cub3d.h"

static int	parse_component(const char **cursor, int *value)
{
	int	result;
	int	digit_count;

	while (is_space_char(**cursor))
		(*cursor)++;
	result = 0;
	digit_count = 0;
	while (is_digit_char(**cursor))
	{
		result = result * 10 + (**cursor - '0');
		(*cursor)++;
		digit_count++;
		if (result > 255)
			return (0);
	}
	if (digit_count == 0)
		return (0);
	while (is_space_char(**cursor))
		(*cursor)++;
	*value = result;
	return (1);
}

int	parse_color_value(const char *line, t_color *color)
{
	const char	*cursor;
	int			red;
	int			green;
	int			blue;

	cursor = line;
	if (!parse_component(&cursor, &red) || *cursor != ',')
		return (0);
	cursor++;
	if (!parse_component(&cursor, &green) || *cursor != ',')
		return (0);
	cursor++;
	if (!parse_component(&cursor, &blue))
		return (0);
	while (is_space_char(*cursor))
		cursor++;
	if (*cursor != '\0')
		return (0);
	color->red = red;
	color->green = green;
	color->blue = blue;
	return (1);
}