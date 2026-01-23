/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/23 09:56:30 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_stash	stash[1024];
	t_lst			*head;
	char			*str;
	size_t			total_len;

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0 || set_state(fd, &stash[fd]) != PROCESSING)
		return (NULL);
	head = new_node(&stash[fd]);
	if (!head)
	{
		stash[fd].state = MALLOC_ERROR;
		return (NULL);
	}
	str = NULL;
	total_len = head->len;
	stash[fd].state = process_buffer(fd, &stash[fd], head, &total_len);
	if (stash[fd].state == MALLOC_ERROR)
	{
		lst_clear(head);
		return (NULL);
	}
	if (stash[fd].state == NEW_LINE_FOUND || stash[fd].state == EOF_READ)
		str = line_from_lst(head, total_len);
	lst_clear(head);
	return (str);
}

// establish the current stash state after last call
t_state	set_state(int fd, t_stash *stash)
{
	if (stash->state == MALLOC_ERROR || stash->state == UNINIT)
	{
		stash->i = 0;
		stash->bytes = 0;
	}
	stash->state = PROCESSING;
	if (stash->i >= stash->bytes)
		stash->state = read_buffer(fd, stash, PROCESSING);
	return (stash->state);
}

// sets buffer and handles read() return
t_state	read_buffer(int fd, t_stash *stash, t_state state)
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
t_state	process_buffer(int fd, t_stash *stash, t_lst *head, size_t *total_len)
{
	t_lst		*ptr;

	ptr = head;
	while (stash->state == PROCESSING)
	{
		if (ptr->nl)
			return (NEW_LINE_FOUND);
		stash->state = read_buffer(fd, stash, PROCESSING);
		if (stash->state == EOF_READ || stash->state == UNINIT)
			return (stash->state);
		ptr->next = new_node(stash);
		if (!ptr->next)
			return (MALLOC_ERROR);
		ptr = ptr->next;
		*total_len += ptr->len;
	}
	return (PROCESSING);
}

// copies the linked list content into a string
char	*line_from_lst(t_lst *head, size_t total_len)
{
	char		*str;
	size_t		j;
	size_t		k;
	t_lst		*ptr;

	if (total_len == 0)
		return (NULL);
	str = malloc(sizeof(char) * (total_len + 1));
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
