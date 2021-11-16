/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/16 22:41:40 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	number;

	i = 0;
	sign = 1;
	number = 0;
	while (str[i] == '\t' || str[i] == '\v' || str[i] == '\r' || str[i] == '\f'
		   || str[i] == '\n' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = (number * 10) + (str[i] - '0');
		i++;
	}
	return (sign * number);
}

int	check_for_error(int argc, char **argv)
{
	if (argc < 4)
	{
		printf("Error : wrong number of arguments");
		return (ERROR);
	}
	if (argv[1] == 0 || ft_atoi(argv[1]) >= 200)
	{
		printf("Error: wrong number of philosophers");
		return (ERROR);
	}
	if (ft_atoi(argv[2]) < 60)
	{
		printf("Error: too little time to die");
		return (ERROR);
	}
	if (ft_atoi(argv[3]) < 60)
	{
		printf("Error: too little time to eat");
		return (ERROR);
	}
	if (ft_atoi(argv[4]) < 60)
	{
		printf("Error: too little time to sleep");
		return (ERROR);
	}
}

unsigned long	get_time(void)
{
	unsigned long	result;
	struct timeval	time;

	gettimeofday(&time, NULL);
	result = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (result);
}

t_arg	*data_init(char **argv)
{
	g_data = malloc(sizeof(t_arg));
	if (!g_data)
		return ((t_arg *)(ERROR));
	g_data->number_of_philo = ft_atoi(argv[1]);
	g_data->time_to_die = ft_atoi(argv[2]);
	g_data->time_to_eat = ft_atoi(argv[3]);
	g_data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		g_data->number_of_times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		g_data->number_of_times_each_philo_must_eat = -1;
	g_data->forks = malloc(sizeof(pthread_mutex_t) * g_data->number_of_philo);
	if (!g_data->forks)
	{
		free(g_data);
		return ((t_arg *)(ERROR));
	}
	pthread_mutex_init(&g_data->print_mutex, NULL);
	g_data->philo = malloc(sizeof(t_philo) * g_data->number_of_philo);
	if (!g_data->philo)
	{
		free(g_data->forks);
		free(g_data);
		return ((t_arg *)(ERROR));
	}
	return (g_data);
}

void	my_usleep(unsigned long wait_time)
{
	unsigned long	check;

	check = get_time();
	while ((get_time() - check) < wait_time)
		usleep(100);
}

void	print_message(int flag, int id, unsigned long time)
{
	unsigned long	time_now;

	time_now = get_time() - time;
	pthread_mutex_lock(&g_data->print_mutex);
	if (flag == 1)
	{
		printf("%ld %d has taken a fork\n", time_now, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return ;
	}
	else if (flag == 2)
	{
		printf("%ld %d is eating\n", time_now, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return ;
	}
	else if (flag == 3)
	{
		printf("%ld %d is sleeping\n", time_now, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return ;
	}
	else if (flag == 4)
	{
		printf("%ld %d is thinking\n", time_now, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return ;
	}
	else if (flag == 5)
	{
		printf("%ld %d died \n", time_now, id);
		return ;
	}
}

void	*philo_actions(void *data) //допилить для ограниченного кол-ва еды
{
	t_philo			*philo;
	int				i;
	int				times_to_eat;
	int				is_alive;

	i = 1;
	is_alive = 1;
	times_to_eat = g_data->number_of_times_each_philo_must_eat;
	if (g_data->number_of_times_each_philo_must_eat < 0)
		i = 0;
	philo = (t_philo *)data;
	philo->left_fork = philo->id - 1;
	if (philo->id == g_data->number_of_philo)
		philo->right_fork = 0;
	else
		philo->right_fork = philo->id;
	//while (1)
	//while (g_data->number_of_times_each_philo_must_eat != 0) // || g_data->number_of_times_each_philo_must_eat > 0)
	while (times_to_eat > 0 && is_alive == 1) //??????
		{
		pthread_mutex_lock(&g_data->forks[philo->right_fork]);
		print_message(1, philo->id, philo->time_from_start);
		pthread_mutex_lock(&g_data->forks[philo->left_fork]);
		print_message(1, philo->id, philo->time_from_start);
		philo->number_of_meals_eaten += 1;
		times_to_eat -= i;
		philo->time_of_last_meal = get_time();
		print_message(2, philo->id, philo->time_from_start);
		my_usleep(g_data->time_to_eat);
		pthread_mutex_unlock(&g_data->forks[philo->left_fork]);
		pthread_mutex_unlock(&g_data->forks[philo->right_fork]);
		print_message(3, philo->id, philo->time_from_start);
		my_usleep(g_data->time_to_sleep);
		print_message(4, philo->id, philo->time_from_start);
	}
}

void	even_tread_create(void)
{
	int	i;

	i = 1;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		g_data->philo[i].number_of_meals_eaten = 0;
		g_data->philo[i].left_fork = 0;
		g_data->philo[i].right_fork = 0;
		pthread_create(&g_data->philo[i].thread, NULL, philo_actions, \
		(void *)&g_data->philo[i]);
		g_data->philo[i].time_from_start = get_time();
		g_data->philo[i].time_of_last_meal = g_data->philo[i].time_from_start;
		usleep(100);
		i += 2;
	}
}

void	odd_tread_create(void)
{
	int	i;

	i = 0;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		g_data->philo[i].number_of_meals_eaten = 0;
		g_data->philo[i].left_fork = 0;
		g_data->philo[i].right_fork = 0;
		pthread_create(&g_data->philo[i].thread, NULL, philo_actions, \
		(void *)&g_data->philo[i]);
		g_data->philo[i].time_from_start = get_time() - 1;
		g_data->philo[i].time_of_last_meal = g_data->philo[i].time_from_start;
		usleep(100);
		i += 2;
	}
}

int	waiter(void)
{
	int				i;
	unsigned long	starving;

	i = 0;
	while (1)
	{
		if (i == g_data->number_of_philo)
			i = 0;
		while (i < g_data->number_of_philo)
		{
			starving = get_time() - g_data->philo[i].time_of_last_meal;
			if (starving > g_data->time_to_die)
			{
				print_message(5, g_data->philo[i].id, \
				g_data->philo[i].time_from_start);
				return (2);
			}
			if (g_data->number_of_times_each_philo_must_eat > 0 && \
				g_data->philo[i].number_of_meals_eaten == \
				g_data->number_of_times_each_philo_must_eat)
//				return (NO_MEALS_LEFT);
//				pthread_join(g_data->philo[i].thread, NULL);
			i++;
		}
		usleep(1000);
	}
}

void	mutex_init(void)
{
	int	i;

	i = 0;
	while (i < g_data->number_of_philo)
	{
		pthread_mutex_init(&g_data->forks[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv) {
	int i;
	int j;
	int check;

	i = 0;
	check_for_error(argc, argv);
	g_data = malloc(sizeof(t_arg));
	data_init(argv);
	mutex_init();
	even_tread_create();
	usleep(500);
	odd_tread_create();
	check = waiter();
	i = 0;
	j = 0;
//	if (check == NO_MEALS_LEFT)
//	{
//		while (j < g_data->number_of_philo)
//		{
//			pthread_join(g_data->philo[j].thread, NULL);
//			j++;
//		}
//		return (SUCCESS);
//	}
	if (check == DEATH)// || check == NO_MEALS_LEFT)
	{
		while (i < g_data->number_of_philo)
		{
			pthread_detach(g_data->philo[i].thread);
			i++;
		}
		return (ERROR);
	}
	return (SUCCESS);
}
