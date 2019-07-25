/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktrout <ktrout@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 08:29:52 by ktrout            #+#    #+#             */
/*   Updated: 2019/07/25 17:26:55 by ktrout           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "get_next_line.h"

static int    verify_newline(char **stack, char **line)
{
    char    *temp_stack;
    char    *check_stack;
    int        i;

    i = 0;
    check_stack = *stack;
    while (check_stack[i] != '\n')
        if (!check_stack[i++])
            return (0);
    temp_stack = &check_stack[i];
    *temp_stack = '\0';
    *line = *stack;
    *stack = ft_strdup(temp_stack + 1);
    return (1);
}

static int    read_file(int fd, char *heap, char **stack, char **line)
{
    int        ret;
    char    *temp_stack;

    while ((ret = read(fd, heap, BUFF_SIZE)) > 0)
    {
        heap[ret] = '\0';
        if (*stack)
        {
            temp_stack = *stack;
            *stack = ft_strjoin(temp_stack, heap);
            free(temp_stack);
            temp_stack = NULL;
        }
        else
            *stack = ft_strdup(heap);
        if (verify_newline(stack, line))
            break ;
    }
    return ((ret > 0) ? 1 : ret);
}

int            get_next_line(const int fd, char **line)
{
    static char        *stack[MAX_FD];
    char            *heap;
    int                ret;
    int                i;

    if (BUFF_SIZE <= 0 || (!line) || fd < 0 || (read(fd, stack[fd], 0) < 0) ||
            (fd >= MAX_FD) || (!(heap = (char *)malloc(sizeof(char) *
            (BUFF_SIZE + 1))))
        return (-1);
    if (stack[fd])
        if (verify_newline(&stack[fd], line))
            return (1);
    i = 0;
    while (i < BUFF_SIZE)
        heap[i++] = '\0';
    ret = read_file(fd, heap, &stack[fd], line);
    free(heap);
    if (ret != 0 || stack[fd] == NULL || stack[fd][0] == '\0')
    {
        *line = ((!ret && *line) ? NULL : *line);
        return (ret);
    }
    *line = stack[fd];
    stack[fd] = NULL;
    return (1);
}
