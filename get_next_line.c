/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/20 18:41:04 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	t_lst			*head;
	char			*str;

	if (stash.state == EOF_READ)
		return (NULL);
	else if (stash.state == UNINIT)
	{
		stash.bytes = read(fd, stash.buf, BUFFER_SIZE);
		if (stash.bytes < 0 || !stash.buf[0])
			return (NULL);
	}
	stash.state = PROCESSING;
	head = new_node('\0');
	if (!head)
		return (NULL);
	str = NULL;
	if (reader(fd, &stash, head))
		str = line_from_lst(head);
	lst_clear(head);
	return (str);
}

// loops over buffer until new line is found, updating stash state
int	reader(int fd, t_stash *stash, t_lst *head)
{
	t_lst		*ptr;

	ptr = head;
	while (stash->state == PROCESSING)
	{
		stash->state = lst_from_buf(stash, &ptr);
		if (stash->state == ERROR)
			return (0);
		if (stash->state == NEW_LINE_FOUND || stash->state == EOF_READ)
			break ;
		else
		{
			ft_bzero(stash->buf, BUFFER_SIZE);
			stash->bytes = read(fd, stash->buf, BUFFER_SIZE);
			if (stash->bytes < 0 || stash->state == ERROR)
				return (0);
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
		(*ptr)->c = stash->buf[stash->i++];
		if ((*ptr)->c == '\n')
			return (NEW_LINE_FOUND);
		else if ((*ptr)->c == '\0')
			return (EOF_READ);
		else
		{
			(*ptr)->next = new_node('\0');
			if (!(*ptr)->next)
				return (ERROR);
			*ptr = (*ptr)->next;
		}
	}
	if (stash->bytes == 0)
		return (EOF_READ);
	else
		return (PROCESSING);
}

// copies the linked list content into a string
char	*line_from_lst(t_lst *head)
{
	char		*str;
	size_t		len;
	size_t		i;

	if (!head->c)
		return (NULL);
	len = ft_lstsize(head);
	str = malloc(sizeof(char) * len + 1);
	if (!str )
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
