/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/08 21:30:37 by caniseed         ###   ########.fr       */
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
	t_arg	*data;

	data = malloc(sizeof(t_arg));
	if (!data)
		exit(ERROR);
	data->number_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->number_of_times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_each_philo_must_eat = 0;
	data->philo = malloc(sizeof(t_philo) * data->number_of_philo);
	if (!data->philo)
	{
		free(data);
		exit(ERROR);
	}
	return (data);
}

unsigned long get_time(void)
{
	unsigned long result;
	struct timeval time;

	gettimeofday(&time, NULL);
	result = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return(result);
}

void	my_sleep(unsigned long time_num)
{
	struct timeval time;

	while (gettimeofday(&time, NULL) < time_num)
		usleep(100);
}

void	*philo_actions(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	philo->time_from_start = get_time();
	philo->time_of_last_meal = get_time();
	printf("%ld\n %ld\n", philo->time_from_start, philo->time_of_last_meal);
	pthread_mutex_lock(&philo->right_fork);
	pthread_mutex_lock(&philo->left_fork);
	printf("%ld Philosopher №%d is eating\n", get_time() - philo->time_from_start, philo->id);
	my_sleep(g_data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	printf("%ld Philosopher №%d is sleeping\n", get_time() - philo->time_from_start, philo->id);
	my_sleep(g_data->time_to_sleep * 1000);
	printf("%ld Philosopher №%d is thinking\n", get_time() - philo->time_from_start, philo->id);
	return (NULL);
}

void even_tread_create(void)
{
	int i;

	i = 1;
	while (i < g_data->number_of_philo)
	{
		g_data->philo->id = i + 1;
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
		g_data->philo->id = i + 1;
		pthread_create(&g_data->philo->thread, NULL, philo_actions, (void *)&g_data->philo[i]);
		i += 2;
	}
}

int	main(int argc, char **argv)
{
//	t_arg	*data;//????

	check_for_error(argc, argv);
	g_data = malloc(sizeof(t_arg));
	data_init(argv);
	even_tread_create();
	odd_tread_create();
	int i = 0;
	while (i < g_data->number_of_philo)
	{
		pthread_join(g_data->philo[i].thread, NULL);
		i++;
	}
	return (SUCCESS);
}