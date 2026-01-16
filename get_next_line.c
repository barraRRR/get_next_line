/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/16 16:51:24 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

typedef struct	s_stash
{
	char			*str;
	int				full_line;
	struct s_stash	*next;
}	t_stash;
/*
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

char	*create_lst_from_buf(t_stash *stash, char *buf)
{
	size_t		i;
	size_t		j;
	size_t		len;
	char		*str;

	len = 0;
	while (len < BUFFER_SIZE && buf[i] != '\n')
	{
		len++;
		i++;
		if (buf[i] == '\n')
		{
			stash = cpy_line(stash, buf, len, i);

	*/			
			

	
char	*my_realloc(char *str, size_t new_size)
{
	char		*tmp;
	size_t		i;
	size_t		j;

	i = 0;
	while(str[i])
		i++;
	tmp = malloc(sizeof(char) * i + new_size + 1);
	j = 0;
	while (str[j])
	{
		tmp[j] = str[j];
		j++;
	}
	while (j < (i + new_size + 1))
		tmp[j++] = '\0';
	free(str);
	return (tmp);
}	

void	go_to_last_node(t_stash **stash)
{
	while ((*stash)->next)
		*stash = (*stash)->next;
}

void	copy_buf_into_stash(t_stash *stash, char *buf, int fd)
{
	size_t		i;

	go_to_last_node(&stash);
	if (stash->full_line)
	{
		stash->next = malloc(sizeof(t_stash));
		if (stash->next)
			return ;
		stash = stash->next;
		stash->full_line = 0;
	}
	else
	{
		
	i = 0;
	while (*tmp != '\n')
	{








char	*get_next_line(int fd)
{
	static t_stash	*stash = NULL;
	char			buf[BUFFER_SIZE];
	char			*str;
	int				bytes;

	if (!stash)
	{
		stash = malloc(sizeof(t_stash *));
		if (!stash)
			return (NULL);
		stash->full_line = 0;
	}
	while (!stash->full_line)
	{
		if (read(fd, buf, BUFFER_SIZE) < 0)
			return (NULL);
		stash = copy_buf_into_stash(stash, buf, fd);




	

	full_line?

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

