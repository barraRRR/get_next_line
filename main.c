/*-----------------------------------------*/
/*------- MAIN TESTER --- jbarreir --------*/
/*-----------------------------------------*/

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static int	print_loop(int fd, char *new_line, int lines)
{
	while (lines)
	{
		new_line = get_next_line(fd);
		if (!new_line)
			return (0);
		printf("%s", new_line);
		free(new_line);
		lines--;
	}
	return (1);
}

static int	interactive_loop(char *new_line, int fd)
{
	int		ok;
	char	enter;

	while (1)
	{
		new_line = get_next_line(fd);
		if (!new_line)
			return (0);
		ok = 1;
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
	return (1);
}

int	main(int argc, char **argv)
{
	char	*new_line;
	int		fd;
	int		lines;

	printf(
	"                __                            __    .__  .__                \n"
	"   ____   _____/  |_    ____   ____ ___  ____/  |_  |  | |__| ____   ____   \n"
	"  / ___\\_/ __ \\   __\\  /    \\_/ __ \\   \\/   /\\  __\\ |  | |  |/    \\_/ __ \\  \n"
	" / /_/  >  ___/|  |   |   |  \\  ___/ >    <  |  |   |  |_|  |   |  \\  ___/  \n"
	" \\___  / \\___  >__|   |___|  /\\___  >__/\\_ \\ |__|   |____/__|___|  /\\___  > \n"
	"/_____/      \\/            \\/     \\/      \\/                     \\/     \\/  \n"
	);

	lines = atoi(argv[1]);
	new_line = NULL;
	if (argc == 1)
	{
		lines = 1;
		fd = 0;
	}	
	else if (argc == 2)
		fd = 0;
	else if (argc == 3)
	{
		fd = open(argv[2], O_RDONLY);
		if (fd < 0 || !print_loop(fd, new_line, lines))
			return (1);
		close(fd);
	}
	else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'i')
	{
		fd = open(argv[2], O_RDONLY);
		if (fd < 0 || !interactive_loop(new_line, fd))
			return (1);
		close(fd);	
	}
	printf("\n-----------------------------------------------");
	printf("\nThanks for evaluating get_next_line by jbarreir\n");
	printf("-----------------------------------------------\n");
	return (0);
}
