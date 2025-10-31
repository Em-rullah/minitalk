
#include "minitalk.h"

void	handler(int sig, siginfo_t *info, void *context)
{
	static unsigned char	bit;
	static int				count;

	(void)info;
	(void)context;
	if (count == 0)
		bit = 0;
	if (sig == SIGUSR2)
		bit |= 1 << (7 - count);
	count++;
	if (count == 8)
	{
		if (bit == 0)
			write(1, "\n", 1);
		else
			write(1, &bit, 1);
		count = 0;
		bit = 0;
	}
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
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
	struct sigaction	act;

	act.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_sigaction = handler;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	ft_putnbr_fd(getpid(), 1);
	ft_putchar_fd('\n', 1);
	while (1)
		pause();
}
