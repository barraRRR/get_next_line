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

        // Esperar entrada del usuario
        while (1)
        {
            if (read(0, &c, 1) != 1)
                return (0);          // stdin cerrado → salir

            if (c == '\n')
                break;               // ENTER

            ok = 0;                  // escribió algo
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

int main(int argc, char **argv)
{
    int fd;
    int lines;

    if (argc == 1)
    {
		print_ascii_art();
        lines = 1;
        fd = 0;
        print_loop(fd, lines);
    }
    else if (argc == 2)
    {
		print_ascii_art();
        lines = atoi(argv[1]);
        fd = 0;
        print_loop(fd, lines);
    }
    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'i')
    {
		print_ascii_art();
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		interactive_loop(fd);
        close(fd);
    }
    else if (argc == 3)
    {
		print_ascii_art();
        lines = atoi(argv[1]);
        fd = open(argv[2], O_RDONLY);
        if (fd < 0 || !print_loop(fd, lines))
            return (1);
        close(fd);
    }

    printf("\n-----------------------------------------------");
    printf("\nThanks for evaluating get_next_line by jbarreir\n");
    printf("-----------------------------------------------\n");
    return (0);
}
