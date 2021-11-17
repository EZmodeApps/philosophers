/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 18:26:23 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/17 18:27:07 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
