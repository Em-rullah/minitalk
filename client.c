/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emkir <emkir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:02:00 by emkir             #+#    #+#             */
/*   Updated: 2025/11/13 11:35:54 by emkir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t	g_sig_control;

static void	handler(sig_atomic_t sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)sig;
	g_sig_control = 1;
}

static void	range_control(long n)
{
	if (-2147483648 > n || n > 2147483647)
		exit(EXIT_FAILURE);
}

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
		range_control(n * sign);
	}
	return (n * sign);
}

static void	send_msg(pid_t pid, unsigned char c)
{
	int	bit;
	int	i;

	g_sig_control = 0;
	i = 7;
	while (i >= 0)
	{
		bit = (c >> i) & 1;
		if (bit == 1)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		while (!g_sig_control)
			pause();
		g_sig_control = 0;
		i--;
	}
}

int	main(int c, char *argv[])
{
	pid_t				pid;
	struct sigaction	sa;

	if (c != 3)
		exit(EXIT_FAILURE);
	pid = ft_atoi(argv[1]);
	if (kill(pid, 0) == -1)
		exit(EXIT_FAILURE);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigaction(SIGUSR1, &sa, NULL);
	while (*argv[2])
	{
		send_msg(pid, *argv[2]);
		argv[2]++;
	}
	send_msg(pid, '\0');
}
