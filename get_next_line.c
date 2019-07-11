/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktrout <ktrout@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 08:29:52 by ktrout            #+#    #+#             */
/*   Updated: 2019/07/11 11:16:37 by ktrout           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "get_next_line.h"

char		*ft_strdup(const char *s1)
{
        char            *s2;
        size_t          i;

        i = 0;
        while (s1[i])
                i += 1;
        if (!(s2 = (char *)malloc(sizeof(char) * (i + 1))))
                return (NULL);
        i = -1;
        while (s1[++i])
                s2[i] = s1[i];
        s2[i] = '\0';
        return (s2);
}

/*
** Allocates with malloc() and returns a “fresh” string ending with ’\0’,
** result of the concatenation of s1 and s2. If the allocation fails the
** function returns NULL.
*/

char		*ft_strjoin(char const *s1, char const *s2)
{
        char            *s3;
        char            *tmp_s3;
        size_t          i;
        size_t          j;

        j = 0;
        i = 0;
        while (s1[i])
                i += 1;
        while (s2[j])
                j += 1;
        if (!s1 || !s2 || !(s3 = (char *)malloc(sizeof(char) * (i + j + 1))))
                return (NULL);
        tmp_s3 = s3;
        while (*s1 != '\0')
                *tmp_s3++ = *s1++;
        while (*s2 != '\0')
                *tmp_s3++ = *s2++;
        *tmp_s3 = '\0';
        return (s3);
}

/*
** This function verifies if the text in the stack has a newline. If it
** doesn't, it returns 0 which indicates that it's not valid.
**
** A temporary stack is made to hold the address of what was checked in the
** text. If there is a newline, a duplicate of the stack is made into the line.
** The temporary stack is then duplicated back into the stack.
*/

static int	verify_newline(char **stack, char **line)
{
	char	*temp_stack;
	char	*check_stack;
	int		i;

	i = 0;
	check_stack = *stack;
	while (check_stack[i] != '\n')
		if (!check_stack[i++])
			return (0);
	temp_stack = &check_stack[i];
	*temp_stack = '\0';
	*line = ft_strdup(*stack);
	*stack = ft_strdup(temp_stack + 1);
	return (1);
}

/*
** This function reads a specific number of bytes (defined by BUFF_SIZE in
** the header file) into the heap from the file descriptors. As long as the
** return value of the read function is > 0 (no errors or if there is
** nothing to read in the file) the function will continue reading through
** the file.
**
** If there is something in the stack, it will be concatenated to what is
** read in the heap. If there is nothing in the stack, the contents of the
** heap will be added to the stack.
**
** The function will then check if there is a newline in the text. If there
** is, it will break from the while loop. If ret is positive, it is forced
** into a 1.
*/

static int	read_file(int fd, char *heap, char **stack, char **line)
{
	int		ret;
	char	*temp_stack;

	ret = read(fd, heap, BUFF_SIZE);
	while (ret > 0)
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

/*
** This GNL function first checks for any errors (if the line is empty,
** if the read return value is negative, if the number of file descriptors
** is invalid or if it fails to allocate the heap) and returns a -1 if
** there are any.
**
** If there is content in the stack, it will check to verify if there is
** a newline. If not, memory is allocated to the heap and the file is read.
**
** When the file is done being read, the heap is freed. Then it checks for
** the value of ret; if it's 1, 1 will be returned, if it's -1, -1 will be
** returned and if the stack is empty, 0 will be returned. If neither of
** these conditions are met, line is set to the value of the stack, the
** stack is set to NULL and it returns 1.
*/

int			get_next_line(const int fd, char **line)
{
	static char		*stack[MAX_FD];
	char			*heap;
	int				ret;
	int				i;

	if (!line || (read(fd, stack[fd], 0) < 0) || (fd < 0 || fd >= MAX_FD) ||
			(!(heap = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1)))))
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
		if (!ret && *line)
			*line = NULL;
		return (ret);
	}
	*line = stack[fd];
	stack[fd] = NULL;
	return (1);
}
