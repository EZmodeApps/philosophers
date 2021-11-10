/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/10 21:06:38 by caniseed         ###   ########.fr       */
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

void	check_for_error(int argc, char **argv)
{
	if (argc < 4)
	{
		printf("Error : wrong number of arguments");
		exit(ERROR);
	}
	if (argv[1] == 0 || ft_atoi(argv[1]) >= 200)
	{
		printf("Error: wrong number of philosophers");
		exit(ERROR);
	}
	if (ft_atoi(argv[2]) < 60)
	{
		printf("Error: too little time to die");
		exit(ERROR);
	}
	if (ft_atoi(argv[3]) < 60)
	{
		printf("Error: too little time to eat");
		exit(ERROR);
	}
	if (ft_atoi(argv[4]) < 60)
	{
		printf("Error: too little time to sleep");
		exit(ERROR);
	}
}

t_arg	*data_init(char **argv)
{
	//t_arg	*g_data;

	g_data = malloc(sizeof(t_arg));
	if (!g_data)
		exit(ERROR);
	g_data->number_of_philo = ft_atoi(argv[1]);
	g_data->time_to_die = ft_atoi(argv[2]);
	g_data->time_to_eat = ft_atoi(argv[3]);
	g_data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		g_data->number_of_times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		g_data->number_of_times_each_philo_must_eat = 0;
	g_data->forks = malloc(sizeof(pthread_mutex_t) * g_data->number_of_philo);
	if (!g_data->forks)
	{
		free(g_data);
		exit(ERROR);
	}
	g_data->philo = malloc(sizeof(t_philo) * g_data->number_of_philo);
	if (!g_data->philo)
	{
		free(g_data->forks);
		free(g_data);
		exit(ERROR);
	}
	return (g_data);
}

unsigned long get_time(void)
{
	unsigned long result;
	struct timeval time;

	gettimeofday(&time, NULL);
	result = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return(result);
}

void	my_sleep(unsigned long time_wait, unsigned long start_time)
{
	unsigned long n;

	n = get_time() - start_time;
	while (n < time_wait)
	{
		n = get_time() - start_time;
		usleep(1000);
	}
}

void	*philo_actions(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	philo->time_from_start = get_time();
	philo->time_of_last_meal = get_time();
	philo->current_time = get_time() - philo->time_from_start;
	philo->left_fork = philo->id;
	if (philo->id == g_data->number_of_philo)
		philo->right_fork = 0;
	else
		philo->right_fork = philo->id + 1;
	while (1)
	{
		pthread_mutex_lock(&g_data->forks[philo->right_fork]);
		printf("%ld %d has taken a fork\n", get_time() - philo->time_from_start, philo->id);
		pthread_mutex_lock(&g_data->forks[philo->left_fork]);
		printf("%ld %d has taken a fork\n", get_time() - philo->time_from_start, philo->id);
		printf("%ld %d is eating\n", get_time() - philo->time_from_start, philo->id);
		philo->is_eating_now = 1;
		philo->number_of_meals_eaten++;
		my_sleep(g_data->time_to_eat, philo->time_from_start);
		//usleep(g_data->time_to_eat);
		pthread_mutex_unlock(&g_data->forks[philo->left_fork]);
		pthread_mutex_unlock(&g_data->forks[philo->right_fork]);
		philo->is_eating_now = 0;
		printf("%ld %d is sleeping\n", get_time() - philo->time_from_start, philo->id);
		my_sleep(g_data->time_to_sleep, philo->time_from_start);
		printf("%ld %d is thinking\n", get_time() - philo->time_from_start, philo->id);
		usleep(1000);
	}
	return (NULL);
}

void even_tread_create(void)
{
	int i;

	i = 1;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		pthread_create(&g_data->philo->thread, NULL, philo_actions, (void *)&g_data->philo[i]);
		i += 2;
	}
}

void odd_tread_create(void)
{
	int i;

	i = 0;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		pthread_create(&g_data->philo->thread, NULL, philo_actions, (void *)&g_data->philo[i]);
		i += 2;
	}
}

int	main(int argc, char **argv)
{
//	t_arg	*data;//????
	int i;

	i = 0;
	check_for_error(argc, argv);
	g_data = malloc(sizeof(t_arg));
	data_init(argv);
	while (i < g_data->number_of_philo)
	{
		pthread_mutex_init(&g_data->forks[i], NULL);
		i++;
	}
	even_tread_create();
	//usleep(1000);
	odd_tread_create();
	i = 0;
	while (i < g_data->number_of_philo)
	{
		pthread_join(g_data->philo[i].thread, NULL);
		i++;
	}
	return (SUCCESS);
}