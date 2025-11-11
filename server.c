/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrul <emrul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:21:07 by emkir             #+#    #+#             */
/*   Updated: 2025/11/11 12:13:53 by emrul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	handler(int sig, siginfo_t *info, void *context)
{
	static unsigned char	bit;
	static int				count;

	(void) context;
	(void) info;
	if (count == 0)
		bit = 0;
	if (sig == SIGUSR2)
		bit |= 1 << (7 - count);
	count++;
	if (count == 8)
	{
		if (bit == '\0')
			write(1, "\n", 1);
		else
			write(1, &bit, 1);
		count = 0;
		bit = 0;
	}
	if (info && info->si_pid > 1)
		kill(info->si_pid, SIGUSR1);
}

static void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

static void	ft_putnbr_fd(int n, int fd)
{
	char	tmp_c;
	long	tmp_n;

	tmp_n = n;
	if (tmp_n < 0)
	{
		ft_putchar_fd('-', fd);
		tmp_n *= -1;
	}
	if (tmp_n / 10 == 0)
	{
		tmp_c = tmp_n + '0';
		ft_putchar_fd(tmp_c, fd);
		return ;
	}
	ft_putnbr_fd(tmp_n / 10, fd);
	tmp_c = tmp_n % 10 + '0';
	ft_putchar_fd(tmp_c, fd);
	return ;
}

int	main(void)
{
	struct sigaction	sa;

	ft_putnbr_fd((int)getpid(), 1);
	ft_putchar_fd('\n', 1);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
}
