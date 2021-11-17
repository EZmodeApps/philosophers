/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:32 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/16 20:25:12 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H
# define PHILO_H

# define SUCCESS 0
# define ERROR 1
# define DEATH 2
# define NO_MEALS_LEFT 3

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_philo
{
	unsigned long	time_from_start;
	unsigned long	time_of_last_meal;
	int				id;
	int				number_of_meals_eaten;
	pthread_t		thread;
	int				right_fork;
	int				left_fork;
	int 			meals_counter;
	int 			finished_eating;
}				t_philo;

typedef struct s_arg
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	int				number_of_times_each_philo_must_eat;
	t_philo			*philo;
}				t_arg;

t_arg	*g_data;

#endif
