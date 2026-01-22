/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbarreir <jbarreir@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:55:06 by jbarreir          #+#    #+#             */
/*   Updated: 2026/01/22 12:51:47 by jbarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// frees linked list
void	lst_clear(t_lst *begin_list)
{
	t_lst	*ptr;

	while (begin_list)
	{
		ptr = begin_list;
		free(begin_list->buf);
		begin_list = begin_list->next;
		free(ptr);
	}
}

t_lst	*new_node(t_stash *stash)
{
	t_lst		*new;

	new = malloc(sizeof(t_lst));
	if (!new)
		return (NULL);
	new->buf = malloc(stash->bytes - stash->i + 1);
	if (!new->buf)
	{
		free(new);
		return (NULL);
	}
	new->len = 0;
	new->nl = 0;
	while (stash->i < stash->bytes)
	{
		new->buf[new->len++] = stash->buf[stash->i++];
		if (new->buf[new->len - 1] == '\n')
		{
			new->nl = 1;
			break ;
		}
	}
	new->next = NULL;
	return (new);
}
