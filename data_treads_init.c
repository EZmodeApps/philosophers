/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_treads_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 18:30:15 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/17 18:30:15 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
