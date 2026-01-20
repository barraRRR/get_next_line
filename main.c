/*-----------------------------------------*/
/*------- MAIN TESTER --- jbarreir --------*/
/*-----------------------------------------*/

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static int	print_loop(int fd, int lines)
{
	char	*new_line;

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

static int interactive_loop(int fd)
{
    char    *line;
    char    c;
    int     ok;

    while ((line = get_next_line(fd)))
    {
        ok = 1;
        while (1)
        {
            if (read(0, &c, 1) != 1)
                return (0);
            if (c == '\n')
                break;
            ok = 0;
        }
        if (!ok)
        {
            free(line);
            return (0);
        }
		printf("%s", line);
        free(line);
    }
    return (1);
}

void	print_ascii_art(void)
{
	printf(
	"                __                            __    .__  .__                \n"
	"   ____   _____/  |_    ____   ____ ___  ____/  |_  |  | |__| ____   ____   \n"
	"  / ___\\_/ __ \\   __\\  /    \\_/ __ \\   \\/   /\\  __\\ |  | |  |/    \\_/ __ \\  \n"
	" / /_/  >  ___/|  |   |   |  \\  ___/ >    <  |  |   |  |_|  |   |  \\  ___/  \n"
	" \\___  / \\___  >__|   |___|  /\\___  >__/\\_ \\ |__|   |____/__|___|  /\\___  > \n"
	"/_____/      \\/            \\/     \\/      \\/                     \\/     \\/  \n\n"
	);
}

void	print_welcome(int argc, int lines)
{
	printf("welcome to get next line tester\n");
	if (argc == 1)
		printf("reading from stdin just one line...\n\n");
	else if (argc == 2)
		printf("reading from stdin %i lines\n\n", lines);
	else if (argc == 3)
		printf("reading from file %i lines\n\n", lines);
}

int main(int argc, char **argv)
{
    int fd;
    int lines;

    // NO ARG: reads one line from stdin
    if (argc == 1)
    {   
		lines = 1;
		print_ascii_art();
		print_welcome(argc, lines);
        fd = 0;
        print_loop(fd, lines);
    }
    // 1 ARG: reads x num of lines from stdin
    else if (argc == 2)
    {
		lines = atoi(argv[1]);
		print_ascii_art();
		print_welcome(argc, lines);
        fd = 0;
        print_loop(fd, lines);
    }
    // 2 ARG: -a + file | reads whole file
	else if (argc == 3  && argv[1][0] == '-' && argv[1][1] == 'a') // quijote
    {
		print_ascii_art();
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		char	*new_line = get_next_line(fd);
			if (!new_line)
				return (0);
		while (new_line)
		{
			printf("%s", new_line);
			free(new_line);
			new_line = get_next_line(fd);
			if (!new_line)
				break ;
		}
        close(fd);
    }
    // 2 ARG: -i + file | interactive mode
    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'i')
    {
		print_ascii_art();
		printf("welcome to get next line tester\n");
		printf("you are in interactive mode\npress enter to get next line...\n");
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		interactive_loop(fd);
        close(fd);
    }
    // 2 ARG: reads x num of lines from file
    else if (argc == 3)
    {
		lines = atoi(argv[1]);
		print_ascii_art();
		print_welcome(argc, lines);
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		print_loop(fd, lines);
        close(fd);
    }

    printf("\n-----------------------------------------------");
    printf("\nThanks for evaluating get_next_line by jbarreir\n");
    printf("-----------------------------------------------\n");
    return (0);
}
