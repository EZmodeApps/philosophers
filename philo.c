/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/17 17:29:11 by caniseed         ###   ########.fr       */
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
		printf("Error : wrong number of arguments\n");
		return (ERROR);
	}
	if (argv[1] == 0 || ft_atoi(argv[1]) >= 200)
	{
		printf("Error: wrong number of philosophers\n");
		return (ERROR);
	}
	if (ft_atoi(argv[2]) < 60 || ft_atoi(argv[3]) < 60 || ft_atoi(argv[4]) < 60)
	{
		printf("Error: too little time to die\n");
		return (ERROR);
	}
	return (OK);
}

unsigned long	get_time(void)
{
	unsigned long	result;
	struct timeval	time;

	gettimeofday(&time, NULL);
	result = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (result);
}

void	data_init_2(void)
{
	pthread_mutex_init(&g_data->print_mutex, NULL);
	g_data->time_from_start = get_time();
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
		g_data->number_of_times_philo_must_eat = ft_atoi(argv[5]);
	else
		g_data->number_of_times_philo_must_eat = -1;
	g_data->forks = malloc(sizeof(pthread_mutex_t) * g_data->number_of_philo);
	if (!g_data->forks)
	{
		free(g_data);
		return ((t_arg *)(ERROR));
	}
	g_data->philo = malloc(sizeof(t_philo) * g_data->number_of_philo);
	if (!g_data->philo)
	{
		free(g_data->forks), free(g_data);
		return ((t_arg *)(ERROR));
	}
	data_init_2();
	return (g_data);
}

void	my_usleep(unsigned long wait_time)
{
	unsigned long	check;

	check = get_time();
	while ((get_time() - check) < wait_time)
		usleep(100);
}

void	print_message_2(int flag, int id, unsigned long time)
{
	if (flag == 4)
	{
		printf("%ld %d is thinking\n", time, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return ;
	}
	else if (flag == 5)
	{
		printf("%ld %d died \n", time, id);
		return ;
	}
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
	print_message_2(flag, id, time_now);
}

void	*philo_actions(void *data)
{
	t_philo			*philo;

	philo = (t_philo *)data;
	while (philo->meals_counter != 0)
	{
		pthread_mutex_lock(&g_data->forks[philo->right_fork]);
		print_message(1, philo->id, g_data->time_from_start);
		pthread_mutex_lock(&g_data->forks[philo->left_fork]);
		print_message(1, philo->id, g_data->time_from_start);
		philo->time_of_last_meal = get_time();
		print_message(2, philo->id, g_data->time_from_start);
		my_usleep(g_data->time_to_eat);
		philo->meals_counter--;
		pthread_mutex_unlock(&g_data->forks[philo->left_fork]);
		pthread_mutex_unlock(&g_data->forks[philo->right_fork]);
		print_message(3, philo->id, g_data->time_from_start);
		my_usleep(g_data->time_to_sleep);
		print_message(4, philo->id, g_data->time_from_start);
	}
	philo->meals_counter = g_data->number_of_times_philo_must_eat;
	return ((void *)FINISHED_EATING);
}

void	even_tread_create(void)
{
	int	i;

	i = 1;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		g_data->philo[i].meals_counter = g_data->number_of_times_philo_must_eat;
		g_data->philo[i].left_fork = g_data->philo[i].id - 1;
		if (g_data->philo[i].id == g_data->number_of_philo)
			g_data->philo[i].right_fork = 0;
		else
			g_data->philo[i].right_fork = g_data->philo[i].id;
		pthread_create(&g_data->philo[i].thread, NULL, philo_actions, \
		(void *)&g_data->philo[i]);
		g_data->philo[i].time_of_last_meal = g_data->time_from_start;
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
		g_data->philo[i].meals_counter = g_data->number_of_times_philo_must_eat;
		g_data->philo[i].left_fork = g_data->philo[i].id - 1;
		if (g_data->philo[i].id == g_data->number_of_philo)
			g_data->philo[i].right_fork = 0;
		else
			g_data->philo[i].right_fork = g_data->philo[i].id;
		pthread_create(&g_data->philo[i].thread, NULL, philo_actions, \
		(void *)&g_data->philo[i]);
		g_data->philo[i].time_of_last_meal = g_data->time_from_start;
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
				g_data->time_from_start);
				return (2);
			}
			if (g_data->number_of_times_philo_must_eat > 0 && \
			g_data->philo[i].meals_counter == 0)
				return (NO_MEALS_LEFT);
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

void	join_thread(void)
{
	int	i;

	i = 0;
	while (i < g_data->number_of_philo)
	{
		pthread_join(g_data->philo[i].thread, NULL);
		i++;
	}
}

void	detach_thread(void)
{
	int	i;

	i = 0;
	while (i < g_data->number_of_philo)
	{
		pthread_detach(g_data->philo[i].thread);
		i++;
	}
}

void	mutex_destroy_and_free(void)
{
	int	i;

	i = 0;
	pthread_mutex_unlock(&g_data->print_mutex);
	pthread_mutex_destroy(&g_data->print_mutex);
	while (i < g_data->number_of_philo)
	{
		pthread_mutex_destroy(&g_data->forks[i]);
		i++;
	}
	free(g_data->forks);
	free(g_data->philo);
	free(g_data);
}

int	main(int argc, char **argv)
{
	int	check;
	int	error;

	error = check_for_error(argc, argv);
	if (error == ERROR)
		return (ERROR);
	data_init(argv);
	mutex_init();
	even_tread_create();
	usleep(1000);
	odd_tread_create();
	check = waiter();
	if (check == NO_MEALS_LEFT)
	{
		join_thread();
		mutex_destroy_and_free();
		return (SUCCESS);
	}
	if (check == DEATH)
	{
		detach_thread();
		mutex_destroy_and_free();
		return (ERROR);
	}
}
