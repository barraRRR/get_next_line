/*-----------------------------------------*/
/*------- MAIN TESTER --- jbarreir --------*/
/*-----------------------------------------*/

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	char	*new_line;
	int		fd;
	int		lines;

	if (argc != 3)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	lines = atoi(argv[2]);
	while (lines)
	{
		new_line = get_next_line(fd);
		if (!new_line)
			return (1);
		printf("%s", new_line);
		free(new_line);
		lines--;
	}
	printf("\n-----------------------------------------------");
	printf("\nThanks for evaluating get_next_line by jbarreir\n");
	printf("-----------------------------------------------\n");
	close(fd);
	return (0);
}
