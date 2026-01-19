/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/19 18:13:53 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	t_lst			*head;
	char			*str;

	if (stash.state == END_OF_FILE_READ)
		return (NULL);
	else if (stash.state == UNINIT)
	{
		flush_buf(stash.buf);		// probabemente reduntandante. investigar
		stash.bytes_read = read(fd, stash.buf, BUFFER_SIZE);
		if (stash.bytes_read < 0)
			return (NULL);
	}
	stash.state = PROCESSING;
	head = malloc(sizeof(t_lst));
	if (!head)
		return (NULL);
	head->next = NULL;
	head->c = '\0';
	while (stash.state == PROCESSING)
	{
		stash.state = lst_from_buf(&stash, head);
		if (stash.state == NEW_LINE_FOUND || stash.state == END_OF_FILE_READ)
			break ;
		else if (!stash.bytes_read && stash.i == BUFFER_SIZE)
			stash.state = END_OF_FILE_READ;
		else
		{
			flush_buf(stash.buf);
			stash.bytes_read = read(fd, stash.buf, BUFFER_SIZE);
			if (stash.bytes_read < 0 || stash.state == ERROR)
			{
				lst_clear(head);
				return (NULL);
			}
			stash.i = 0;
		}
	}
	str = line_from_lst(head);
	lst_clear(head);
	return (str);
}

// reads BUFFER_SIZE bytes and creates a linked list
t_state	lst_from_buf(t_stash *stash, t_lst *ptr)
{
	while (stash->i < BUFFER_SIZE)
	{
		if (stash->buf[stash->i] == '\0' || stash->bytes_read == 0)
			return (END_OF_FILE_READ);
		ptr->c = stash->buf[stash->i++];
		if (ptr->c == '\n')
			return (NEW_LINE_FOUND);
		else
		{
			ptr->next = malloc(sizeof(t_lst));
			if (!ptr->next)
				return (ERROR);
			ptr = ptr->next;
			ptr->c = '\0';
			ptr->next = NULL;
		}
	}
	return (PROCESSING);
}

// copies the linked list content into a string
char	*line_from_lst(t_lst *head)
{
	char		*str;
	size_t		len;
	size_t		i;

	len = ft_lstsize(head);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (head && i < len)
	{
		str[i++] = head->c;
		if (i < len)
			head = head->next;
	}
	str[len] = '\0';
	return (str);
}
