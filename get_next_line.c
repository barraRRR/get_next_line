/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:48:48 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/18 18:18:53 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	t_lst			*head;
	t_lst			*ptr;
	char			*str;

	if (fd == -1)
	{
		free(stash.buf_in);
		return (NULL);
	}
	else if (stash.state == END_OF_FILE_READ)
		return (NULL);
	else if (stash.state == UNINIT && fd != 0)
	{
		stash.bytes_read = read(fd, stash.buf, BUFFER_SIZE);
		if (stash.bytes_read < 0)
			return (NULL);
	}
	else if (stash.state == UNINIT && fd == 0)
	{
		head = lst_from_in(&stash);
		stash.buf_in = line_from_lst(head);
		free(head);
	}
	stash.state = PROCESSING;
	head = malloc(sizeof(t_lst));
	if (!head)
		return (NULL);
	ptr = head;
	if (fd == 0)
		while (stash.state == PROCESSING)
			stash.state = lst_from_buf(stash.buf_in, &ptr, &stash.i);
	else
	{
		while (stash.state == PROCESSING)
		{
			stash.state = lst_from_buf(stash.buf, &ptr, &stash.i);
			if (stash.state == NEW_LINE_FOUND || stash.state == END_OF_FILE_READ)
			{
				ptr = head;
				break ;
			}
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
	}
	str = line_from_lst(head);
	lst_clear(head);
	return (str);
}

// reads BUFFER_SIZE bytes and creates a linked list
t_state	lst_from_buf(char *buf, t_lst **ptr, size_t *i)
{
	while (buf[*i] && *i < BUFFER_SIZE)
	{
		(*ptr)->c = buf[(*i)++];
		if ((*ptr)->c == '\n')
			return (NEW_LINE_FOUND);
		else if ((*ptr)->c == '\0')
			return (END_OF_FILE_READ);
		else
		{
			(*ptr)->next = malloc(sizeof(t_lst));
			if (!(*ptr)->next)
				return (ERROR);
			*ptr = (*ptr)->next;
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
		str[i++] = ptr->c;
		ptr = ptr->next;			// duda si hacemos segfault aqui
	}
	str[len] = '\0';
	return (str);
}

// reads from stdin into an allocated buffer
t_lst	*lst_from_in(t_stash *stash)
{
	ssize_t		bytes;
	t_lst		*head;
	t_lst		*ptr;

	bytes = 1;
	head = malloc(sizeof(t_lst));
	if (!head)
		return (NULL);
	ptr = head;
	while (bytes > 0)
	{
		stash->i = 0;
		flush_buf(stash->buf);
		bytes = read(0, stash->buf, BUFFER_SIZE);
		while ((int)stash->i < bytes)
		{
			ptr->c = stash->buf[stash->i++];
			if ((int)stash->i < bytes || bytes == BUFFER_SIZE)
			{
				ptr->next = malloc(sizeof(t_lst));
				if (!ptr->next)
					return (NULL);
				ptr = ptr->next;
			}
			else
				ptr->next = (NULL);
		}
	}
	if (bytes < 0)
	{
		lst_clear(head);
		return (NULL);
	}
	return (head);
}

/*
t_lst   *lst_from_in(t_stash *stash)
{
    ssize_t     bytes;
    t_lst       *head = NULL;
    t_lst       *ptr = NULL;

    while ((bytes = read(0, stash->buf, BUFFER_SIZE)) > 0)
    {
        stash->i = 0;
        while ((int)stash->i < bytes)
        {
            t_lst *new_node = malloc(sizeof(t_lst));
            if (!new_node)
                return (lst_clear(head), NULL); // Limpiar si falla el malloc
            
            new_node->c = stash->buf[stash->i++];
            new_node->next = NULL;

            if (!head) // Primer nodo
                head = new_node;
            else
                ptr->next = new_node;
            
            ptr = new_node;
        }
    }

    if (bytes < 0) // Error en read
        return (lst_clear(head), NULL);

    return (head);
}
*/