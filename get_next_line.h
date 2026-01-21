/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:52:39 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/21 18:37:37 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef enum e_state
{
	UNINIT,
	PROCESSING,
	NEW_LINE_FOUND,
	EOF_READ,
	MALLOC_ERROR
}	t_state;

typedef struct s_lst
{
	char			c;
	struct s_lst	*next;
}	t_lst;

typedef struct s_stash
{
	t_state			state;
	ssize_t			bytes;
	ssize_t			i;
	char			buf[BUFFER_SIZE];
}	t_stash;

char		*get_next_line(int fd);
t_state		reader(int fd, t_stash *stash, t_lst *head, size_t *len);
void		set_state(int fd, t_stash *stash);
char		*line_from_lst(t_lst *head, size_t len);
t_lst		*new_node(char c);
void		lst_clear(t_lst *begin_list);
t_state		flush_and_read(int fd, t_stash *stash, t_state state);

#endif
