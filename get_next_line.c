/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/21 22:19:36 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	t_lst			*head;
	char			*str;

	set_state(fd, &stash);
	if (stash.state != PROCESSING)
		return (NULL);
	head = new_node(&stash);
	if (!head)
		return (NULL);
	str = NULL;
	stash.state = reader(fd, &stash, head);
	if (stash.state == MALLOC_ERROR)
	{
		lst_clear(head);
		return (NULL);
	}
	if (stash.state == NEW_LINE_FOUND || stash.state == EOF_READ)
		str = line_from_lst(stash, head);
	lst_clear(head);
	return (str);
}

// establish the current stash state after last call
void	set_state(int fd, t_stash *stash)
{
	if (stash->state == MALLOC_ERROR || stash->state == UNINIT)
	{
		stash->i = 0;
		stash->bytes = 0;
	}
	stash->state = PROCESSING;
	if (stash->i >= stash->bytes)
		stash->state = flush_and_read(fd, stash, PROCESSING);
}

// sets buffer and handles read() return
t_state	flush_and_read(int fd, t_stash *stash, t_state state)
{
	stash->i = 0;
	stash->bytes = read(fd, stash->buf, BUFFER_SIZE);
	if (stash->bytes < 0)
	{
		stash->bytes = 0;
		return (UNINIT);
	}
	else if (stash->bytes == 0)
		return (EOF_READ);
	return (state);
}

// loops over buffer until new line is found, updating stash state
t_state	reader(int fd, t_stash *stash, t_lst *head)
{
	t_lst		*ptr;
	size_t		j;

	ptr = head;
	while (stash->state == PROCESSING)
	{
		j = 0;
		while ((ssize_t)j < stash->bytes)
		{
			if (ptr->buf[j] == '\n')
				return (NEW_LINE_FOUND);
			j++;
		}
		stash->state = flush_and_read(fd, stash, PROCESSING);
		if (stash->state == EOF_READ || stash->state == UNINIT)
			return (stash->state);
		ptr->next = new_node(stash);
		if (!ptr->next)
			return (MALLOC_ERROR);
		ptr = ptr->next;
	}
	return (PROCESSING);
}

// copies the linked list content into a string
char	*line_from_lst(t_stash stash, t_lst *head)
{
	char		*str;
	size_t		j;
	size_t		k;
	t_lst		*ptr;
	size_t		len;

	len = gnl_strlen(head);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	j = 0;
	ptr = head;
	while (ptr)
	{
		k = 0;
		while (k < ptr->len)
			str[j++] = ptr->buf[k++];
		ptr = ptr->next;
	}
	str[j] = '\0';
	return (str);
}
