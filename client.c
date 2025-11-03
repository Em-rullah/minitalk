/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrul <emrul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:02:00 by emkir             #+#    #+#             */
/*   Updated: 2025/11/03 12:01:19 by emrul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	ft_atoi(const char *nptr)
{
	int		i;
	int		sign;
	long	n;

	i = 0;
	sign = 1;
	n = 0;
	while ((nptr[i] == ' ' || nptr[i] == '\f' || nptr[i] == '\n'
			|| nptr[i] == '\r' || nptr[i] == '\t' || nptr[i] == '\v'))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while ('0' <= nptr[i] && nptr[i] <= '9')
	{
		n = 10 * n + (nptr[i] - '0');
		i++;
	}
	return (n * sign);
}

static void	send_msg(int pid, unsigned char c)
{
	int	bit;
	int	i;

	i = 7;
	while (i >= 0)
	{
		bit = (c >> i) & 1;
		if (bit == 1)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		i--;
		usleep(150);
	}
}

int	main(int c, char *argv[])
{
	pid_t	pid;

	if (c != 3)
		exit(EXIT_FAILURE);
	pid = ft_atoi(argv[1]);
	if (kill(pid, 0) == -1)
		exit(EXIT_FAILURE);
	while (*argv[2])
	{
		send_msg(pid, *argv[2]);
		argv[2]++;
	}
	send_msg(pid, '\0');
}
