/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/16 15:39:20 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

typedef struct	s_stash
{
	char			buf[BUFFER_SIZE];
	size_t			read;
	struct s_stash	*next;
}	t_stash;

size_t	str_stash_len(t_stash *stash)
{
	size_t		len;
	size_t		i;

	len = 0;
	while (stash->next)
	{
		len += BUFFER_SIZE;
		stash = stash->next;
	}
	i = 0;
	while (stash->buf[i] && i < BUFFER_SIZE && stash->buf[i] != '\n')
	{
		i++;
		len++;
	}
	if (stash->buf[i] == '\n')
		len++;
	return (len);
}

int	find_new_line_in_buf(char *buf) // plantear si debe rastrear por NULL
{
	size_t		i;

	i = 0;
	while (buf[i] && i < BUFFER_SIZE)
	{
		if (buf[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int	find_new_line_in_stash(t_stash *stash)
{
	while (stash)
	{
		if (find_new_line_in_buf(stash->buf))
			return (1);
		stash = stash->next;
	}
	return (0);
}

t_stash	*add_ptr_back(t_stash *stash)
{
	t_stash		*ptr;

	while (stash->next)
		stash = stash->next;
	stash->next = ptr;
	return (ptr);
}

char	*cpy_stash(t_stash *stash)
{
	size_t		i;
	size_t		j;
	size_t		len;
	char		*str;

	len = str_stash_len(stash);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		j = 0;
		while (i < len && j < BUFFER_SIZE)
			str[i++] = stash->buf[j++];
		
			

	


char	*get_next_line(int fd)
{
	static t_stash	*stash = NULL;
	t_stash			*ptr;
	char			*str;
	int				bytes;

	if (!stash)
	{
		stash = malloc(sizeof(t_stash *));
		if (!stash)
			return (NULL);
		if (read(fd, ptr->buf, BUFFER_SIZE) < 0)
			return (NULL);
	}
	if (find_new_line_in_stash(stash))
	{
		str = cpy_stash(stash);
		return (str);
	}
	else
	{
		while (!find_new_line_in_buf(ptr->buf))
		{
			ptr = add_ptr_back(stash);
			ptr = malloc(sizeof(t_stash);
			bytes = read(fd, ptr->buf, BUFFER_SIZE);
			if (bytes < 0)
			{
				// free list
				return (NULL);
			}
		}
	}
	str = cpy_stash(stash);
	return (str);
}

