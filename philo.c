/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/21 23:05:01 by caniseed         ###   ########.fr       */
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

void	*philo_actions(void *data, t_arg *main_data)
{
	t_philo			*philo;

	philo = (t_philo *)data;
	while (philo->meals_counter != 0)
	{
		pthread_mutex_lock(&philo->data_main->forks[philo->right_fork]);
		print_message(1, philo->id, main_data->time_from_start, main_data);
		pthread_mutex_lock(&philo->data_main->forks[philo->left_fork]);
		print_message(1, philo->id, main_data->time_from_start, main_data);
		philo->time_of_last_meal = get_time();
		print_message(2, philo->id, main_data->time_from_start, main_data);
		my_usleep(main_data->time_to_eat);
		philo->meals_counter--;
		pthread_mutex_unlock(&philo->data_main->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data_main->forks[philo->right_fork]);
		print_message(3, philo->id, main_data->time_from_start, main_data);
		my_usleep(main_data->time_to_sleep);
		print_message(4, philo->id, main_data->time_from_start, main_data);
	}
	philo->meals_counter = main_data->number_of_times_philo_must_eat;
	return ((void *)FINISHED_EATING);
}

int	waiter(t_arg *main_data)
{
	int				i;
	unsigned long	starving;

	i = 0;
	while (1)
	{
		if (i == main_data->number_of_philo)
			i = 0;
		while (i < main_data->number_of_philo)
		{
			starving = get_time() - main_data->philo[i].time_of_last_meal;
			if ((int)starving > main_data->time_to_die)
			{
				print_message(5, main_data->philo[i].id, \
				main_data->time_from_start, main_data);
				return (DEATH);
			}
			if (main_data->number_of_times_philo_must_eat > 0 && \
			main_data->philo[i].meals_counter == 0)
				return (NO_MEALS_LEFT);
			i++;
		}
		usleep(1000);
	}
}

int	main(int argc, char **argv)
{
	int		check;
	int		error;
	t_arg	*main_data;

	error = check_for_error(argc, argv);
	if (error == ERROR)
		return (ERROR);
	main_data = malloc(sizeof(t_arg));
	if (!main_data)
		return (ERROR);
	data_init(argv, main_data);
	mutex_init(main_data);
	even_thread_create(main_data);
	usleep(1000);
	odd_thread_create(main_data);
	check = waiter(main_data);
	if (check == NO_MEALS_LEFT)
	{
		join_thread(main_data);
		mutex_destroy_and_free(main_data);
		return (SUCCESS);
	}
	if (check == DEATH)
	{
		detach_thread(main_data);
		pthread_mutex_unlock(&main_data->print_mutex);
		mutex_destroy_and_free(main_data);
		return (ERROR);
	}
}
