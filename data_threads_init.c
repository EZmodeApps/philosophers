/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_threads_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:18:06 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/21 19:41:13 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	data_init_2(t_arg *main_data)
{
	pthread_mutex_init(&main_data->print_mutex, NULL);
	main_data->time_from_start = get_time();
}

t_arg	*data_init(char **argv, t_arg *main_data)
{
	//g_data = malloc(sizeof(t_arg));
//	if (!g_data)
//		return ((t_arg *)(ERROR));
	main_data->number_of_philo = ft_atoi(argv[1]);
	main_data->time_to_die = ft_atoi(argv[2]);
	main_data->time_to_eat = ft_atoi(argv[3]);
	main_data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		main_data->number_of_times_philo_must_eat = ft_atoi(argv[5]);
	else
		main_data->number_of_times_philo_must_eat = -1;
	main_data->forks = malloc(sizeof(pthread_mutex_t) * main_data->number_of_philo);
	if (!main_data->forks)
	{
		free(main_data);
		return ((t_arg *)(ERROR));
	}
	main_data->philo = malloc(sizeof(t_philo) * main_data->number_of_philo);
	if (!main_data->philo)
	{
		free(main_data->forks),
		free(main_data);
		return ((t_arg *)(ERROR));
	}
	data_init_2(main_data);
	return (main_data);
}

void	even_thread_create(t_arg *main_data)
{
	int	i;

	i = 1;
	while (i < main_data->number_of_philo)
	{
		main_data->philo[i].id = i + 1;
		main_data->philo[i].meals_counter = main_data->number_of_times_philo_must_eat;
		main_data->philo[i].left_fork = main_data->philo[i].id - 1;
		if (main_data->philo[i].id == main_data->number_of_philo)
			main_data->philo[i].right_fork = 0;
		else
			main_data->philo[i].right_fork = main_data->philo[i].id;
		pthread_create(&main_data->philo[i].thread, NULL, philo_actions, \
		(void *)&main_data->philo[i]);
		main_data->philo[i].time_of_last_meal = main_data->time_from_start;
		usleep(100);
		i += 2;
	}
}

void	odd_thread_create(t_arg *main_data)
{
	int	i;

	i = 0;
	while (i < main_data->number_of_philo)
	{
		main_data->philo[i].id = i + 1;
		main_data->philo[i].meals_counter = main_data->number_of_times_philo_must_eat;
		main_data->philo[i].left_fork = main_data->philo[i].id - 1;
		if (main_data->philo[i].id == main_data->number_of_philo)
			main_data->philo[i].right_fork = 0;
		else
			main_data->philo[i].right_fork = main_data->philo[i].id;
		pthread_create(&main_data->philo[i].thread, NULL, philo_actions, \
		(void *)&main_data->philo[i]);
		main_data->philo[i].time_of_last_meal = main_data->time_from_start;
		usleep(100);
		i += 2;
	}
}
