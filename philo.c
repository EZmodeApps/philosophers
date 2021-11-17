/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/17 18:32:26 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_for_error(int argc, char **argv)
{
	if (argc < 4)
	{
		printf("Error : wrong number of arguments!\n");
		return (ERROR);
	}
	if (argv[1] == 0 || ft_atoi(argv[1]) >= 200)
	{
		printf("Error: wrong number of philosophers!\n");
		return (ERROR);
	}
	if (ft_atoi(argv[2]) < 60 || ft_atoi(argv[3]) < 60 || ft_atoi(argv[4]) < 60)
	{
		printf("Error: bad argument!\n");
		return (ERROR);
	}
	return (OK);
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
			if ((int)starving > g_data->time_to_die)
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
