#include "cub3d.h"

size_t	string_length(const char *string)
{
	size_t	length;

	length = 0;
	while (string && string[length])
		length++;
	return (length);
}

char	*string_duplicate(const char *string)
{
	char	*copy;
	size_t	length;

	length = string_length(string);
	copy = malloc(length + 1);
	if (!copy)
		return (NULL);
	memcpy(copy, string, length);
	copy[length] = '\0';
	return (copy);
}

char	*string_join(const char *left, const char *right)
{
	char	*joined;
	size_t	left_length;
	size_t	right_length;

	left_length = string_length(left);
	right_length = string_length(right);
	joined = malloc(left_length + right_length + 1);
	if (!joined)
		return (NULL);
	memcpy(joined, left, left_length);
	memcpy(joined + left_length, right, right_length);
	joined[left_length + right_length] = '\0';
	return (joined);
}

char	*string_substr(const char *string, size_t start, size_t len)
{
	char	*substring;
	size_t	string_length_value;

	string_length_value = string_length(string);
	if (start >= string_length_value)
		return (string_duplicate(""));
	if (len > string_length_value - start)
		len = string_length_value - start;
	substring = malloc(len + 1);
	if (!substring)
		return (NULL);
	memcpy(substring, string + start, len);
	substring[len] = '\0';
	return (substring);
}

int	string_compare(const char *left, const char *right)
{
	while (*left && *right && *left == *right)
	{
		left++;
		right++;
	}
	return ((unsigned char)*left - (unsigned char)*right);
}

int	is_space_char(int character)
{
	return (character == ' ' || character == '\t' || character == '\n'
		|| character == '\r' || character == '\v' || character == '\f');
}

int	is_digit_char(int character)
{
	return (character >= '0' && character <= '9');
}

char	*trim_left(char *string)
{
	while (string && *string && is_space_char(*string))
		string++;
	return (string);
}

void	trim_right_in_place(char *string)
{
	size_t	length;

	if (!string)
		return ;
	length = string_length(string);
	while (length > 0 && is_space_char(string[length - 1]))
	{
		string[length - 1] = '\0';
		length--;
	}
}