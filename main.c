/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:26:38 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/23 10:44:46 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	interactive_loop(int fd)
{
	char		*line;
	char		c;
	int			ok;

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

void	print_welcome(int option, int lines)
{
	printf(
	"\n                __                            __    .__  .__                \n"
	"   ____   _____/  |_    ____   ____ ___  ____/  |_  |  | |__| ____   ____   \n"
	"  / ___\\_/ __ \\   __\\  /    \\_/ __ \\   \\/   /\\  __\\ |  | |  |/    \\_/ __ \\  \n"
	" / /_/  >  ___/|  |   |   |  \\  ___/ >    <  |  |   |  |_|  |   |  \\  ___/  \n"
	" \\___  / \\___  >__|   |___|  /\\___  >__/\\_ \\ |__|   |____/__|___|  /\\___  > \n"
	"/_____/      \\/            \\/     \\/      \\/                     \\/     \\/  \n\n"
	);
	printf(
	"┌──────────────────────────────────────────┐\n"
	"│          WELCOME TO GNL TESTER           │\n"
	"└──────────────────────────────────────────┘\n"
	"                 jbarreir\n\n"
	);
	if (option == 1)
		printf("reading from stdin just one line...\n");
	else if (option == 2)
		printf("reading from stdin %i lines\n", lines);
	else if (option == 3)
		printf("reading from file %i lines\n", lines);
	else if (option == 4)
		printf("reading all lines from file...\n");
	else if (option == 5)
		printf("you are in interactive mode\npress enter to get next line...\n");
	printf("--------------------------------------------\n\n");
}

int main(int argc, char **argv)
{
    int fd;
    int lines;

    if (argc == 1)
    {   // NO ARG: reads one line from stdin
		print_welcome(1, 1);
        print_loop(0, 1);
    }
    else if (argc == 2)
    {	// 1 ARG: reads x num of lines from stdin
		lines = atoi(argv[1]);
		print_welcome(2, lines);
        print_loop(0, lines);
    }
	else if (argc == 3  && argv[1][0] == '-' && argv[1][1] == 'a')
    {	// 2 ARG: -a + file | reads whole file
		print_welcome(4, 0);
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
    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'i')
    {	// 2 ARG: -i + file | interactive mode
		print_welcome(5, 0);
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		interactive_loop(fd);
        close(fd);
    }
    else if (argc == 3)
    {	// 2 ARG: reads x num of lines from file
		lines = atoi(argv[1]);
		print_welcome(3, lines);
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
