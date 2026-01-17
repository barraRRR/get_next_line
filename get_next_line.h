/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:52:39 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/17 12:51:00 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef enum	e_state
{
	UNINIT,
	PROCESSING,
	NEW_LINE_FOUND,
	END_OF_FILE_READ,
	ERROR
}	t_state;

typedef struct	s_stash
{
	char			buf[BUFFER_SIZE];
	int				buf_fd;
	t_state			state;
	size_t			i;
}	t_stash;

typedef struct	s_lst
{
	char			c;
	struct s_lst	*next;
}	t_lst;

#endif
