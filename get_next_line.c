/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/17 13:08:16 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	t_lst			*head;
	char			*str;
	int				bytes;

	if (stash.state == UNINIT || stash.buf_fd != fd)
	{
		bytes = read(fd, stash.buf, BUFFER_SIZE);
		if (bytes < 0)
			return (NULL);
		stash.buf_fd = fd;
	}
	else if (stash.state == END_OF_FILE_READ)
		return (NULL);
	stash.state = PROCESSING;
	head = malloc(sizeof(t_lst));
	if (!head)
		return (NULL);
	while (stash.state == PROCESSING)
	{
		stash.state = lst_from_buf(stash, head);
		if (bytes == 0)
			stash.state == END_OF_FILE_READ;
		bytes = read(fd, stash.buf, BUFFER_SIZE);
		if (bytes < 0 || stash.state == ERROR)
			return (NULL);
		stash.i = 0;
	}
	str = line_from_lst(head);
	lst_clear(head);
	return (str);
}

// reads BUFFER_SIZE bytes and creates a linked list
t_state	*lst_from_buf(t_stash stash, t_lst *ptr)
{
	while (stash.i < BUFFER_SIZE)
	{
		ptr->c = stash.buf[stash.i++];
		if (ptr->c == '\n')
			return (NEW_LINE_FOUND);
		else if (ptr->c == '\0')
			return (END_OF_FILE_READ);
		else
		{
			ptr->next = malloc(sizeof(t_lst));
			if (!ptr->next)
				return (ERROR);
			ptr = ptr->next;
		}
	}
	return (PROCESSING);
}

// copies the linked list content into a string
char	*line_from_lst(t_lst *head)
{
	char		*str;
	t_lst		*ptr;
	size_t		len;
	size_t		i;

	len = 0;
	ptr = head;
	while (ptr)
	{
		ptr = ptr->next;
		len++;
	}
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	i = 0;
	ptr = head;
	while (i < len)
	{
		str[i++] == ptr->c;
		ptr = ptr->next;			// duda si hacemos segfault aqui
	}
	str[len] = '\0';
	return (str);
}

// frees linked list
void	lst_clear(t_lst *begin_list)
{
	t_lst	*ptr;

	while (begin_list)
	{
		ptr = begin_list;
		begin_list = begin_list->next;
		free(ptr);
	}
}
