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
	char	enter;
	int		ok;

	if (argc == 1)
	{
		fd = 0;
		printf("Enter your text and press Ctr + D (then press enter to get next line): ");
	}
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			return (1);
		}
		printf("Press enter to get next line...\n");

	}
	while (1)
	{
		ok = 1;
		new_line = get_next_line(fd);
		if (!new_line)
			return (1);
		while (read(0, &enter, 1) == 1 && enter != '\n')
			ok = 0;
		if (!ok)
		{
			free(new_line);
			break ;
		}
		printf("%s", new_line);
		free(new_line);
	}
	if (fd == 0)
		new_line = get_next_line(-1);
	printf("\n-----------------------------------------------");
	printf("\nThanks for evaluating get_next_line by jbarreir\n");
	printf("-----------------------------------------------\n");
	close(fd);
	return (0);
}
