/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:43:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/23 11:12:14 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	close_and_free(int *fd, int i, int error)
{
	while (i >= 0)
		close(fd[i--]);
	free(fd);
	return (error);
}

int main(int argc, char **argv)
{
    int 	*fd;
	int		i;

	fd = malloc(sizeof(int) * (argc - 1));
	if (!fd)
		return (1);
	i = 0;
	while (i < (argc - 1))
	{
		fd[i] = open(argv[i + 1], O_RDONLY);
		if (fd[i] < 0)
			return (close_and_free(fd, i, 1));
		i++;
	}
	printf(
	"\n┌──────────────────────────────────────────┐\n"
	"│        WELCOME TO GNL BONUS TESTER       │\n"
	"└──────────────────────────────────────────┘\n"
	"                 jbarreir\n"
	);
	i = 0;
	while (i < (argc - 1))
	{
		printf("\n\nreading from: %s\n\n", argv[i + 1]);
		char	*new_line = get_next_line(fd[i]);
		if (!new_line)
			break ;
		while (new_line)
		{
			printf("%s", new_line);
			free(new_line);
			new_line = get_next_line(fd[i]);
			if (!new_line)
				break ;
		}
		i++;
    }
    printf("\n-----------------------------------------------");
    printf("\nThanks for evaluating get_next_line by jbarreir\n");
    printf("-----------------------------------------------\n");
    return (close_and_free(fd, i, 0));
}
