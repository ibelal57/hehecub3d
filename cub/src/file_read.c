#include "cub3d.h"

char	*read_entire_file(const char *path)
{
	int		fd;
	char	buffer[4097];
	char	*content;
	char	*chunk;
	ssize_t	bytes_read;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	content = string_duplicate("");
	if (!content)
		return (close(fd), NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, 4096);
		if (bytes_read < 0)
			return (free(content), close(fd), NULL);
		buffer[bytes_read] = '\0';
		chunk = string_join(content, buffer);
		free(content);
		content = chunk;
		if (!content)
			return (close(fd), NULL);
	}
	close(fd);
	return (content);
}