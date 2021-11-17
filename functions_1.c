/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 18:19:02 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/17 18:19:02 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time(void)
{
	unsigned long	result;
	struct timeval	time;

	gettimeofday(&time, NULL);
	result = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (result);
}

void	my_usleep(unsigned long wait_time)
{
	unsigned long	check;

	check = get_time();
	while ((get_time() - check) < wait_time)
		usleep(100);
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
