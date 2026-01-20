/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/20 16:34:12 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	t_lst			*head;
	t_lst			*ptr;
	char			*str;

	if (stash.state == EOF_READ)
		return (NULL);
	else if (stash.state == UNINIT)
	{
		stash.bytes_read = read(fd, stash.buf, BUFFER_SIZE);
		if (stash.bytes_read < 0 || (!stash.bytes_read && !stash.buf[0]))
			return (NULL);
	}
	stash.state = PROCESSING;
	head = malloc(sizeof(t_lst));
	if (!head)
		return (NULL);
	head->next = NULL;
	ptr = head;
	if (!reader(fd, &stash, head, &ptr))
		return (NULL);
	str = line_from_lst(head);
	if (str)
		lst_clear(head);
	return (str);
}

// loops over buffer until new line is found, updating stash state
int	reader(int fd, t_stash *stash, t_lst *head, t_lst **ptr)
{
	while (stash->state == PROCESSING)
	{
		stash->state = lst_from_buf(stash, ptr);
		if (stash->state == ERROR)
		{
			lst_clear(head);
			return (0);
		}
		else if (stash->state == NEW_LINE_FOUND || stash->state == EOF_READ)
			break ;
		else if (stash->bytes_read == 0 && stash->i == BUFFER_SIZE)
			stash->state = EOF_READ;
		else
		{
			flush_buf(stash->buf);
			stash->bytes_read = read(fd, stash->buf, BUFFER_SIZE);
			if (stash->bytes_read < 0 || stash->state == ERROR)
			{
				lst_clear(head);
				return (0);
			}
			stash->i = 0;
		}
	}
	return (1);
}

// reads BUFFER_SIZE bytes and creates a linked list
t_state	lst_from_buf(t_stash *stash, t_lst **ptr)
{
	while (stash->i < BUFFER_SIZE)
	{
		if (stash->buf[stash->i] == '\0' || stash->bytes_read == 0)
			return (EOF_READ);
		(*ptr)->c = stash->buf[stash->i++];
		if ((*ptr)->c == '\n')
			return (NEW_LINE_FOUND);
		else
		{
			(*ptr)->next = malloc(sizeof(t_lst));
			if (!(*ptr)->next)
				return (ERROR);
			*ptr = (*ptr)->next;
			(*ptr)->c = '\0';
			(*ptr)->next = NULL;
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

	if (!head->c)
	{
		lst_clear(head);
		return (NULL);
	}
	len = ft_lstsize(head);
	str = malloc(sizeof(char) * len + 1);
	if (!str )
	{
		lst_clear(head);
		return (NULL);
	}
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
