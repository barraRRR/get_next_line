/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:52:39 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/20 18:20:13 by jbarreir         ###   ########.fr       */
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
	ERROR
}	t_state;

typedef struct s_stash
{
	t_state			state;
	ssize_t			bytes;
	size_t			i;
	char			buf[BUFFER_SIZE];
}	t_stash;

typedef struct s_lst
{
	char			c;
	struct s_lst	*next;
}	t_lst;

char		*get_next_line(int fd);
int			reader(int fd, t_stash *stash, t_lst *head);
t_state		lst_from_buf(t_stash *stash, t_lst **ptr);
char		*line_from_lst(t_lst *head);
int			ft_lstsize(t_lst *lst);
t_lst		*new_node(char c);
void		lst_clear(t_lst *begin_list);
void		ft_bzero(void *s, size_t n);

#endif
