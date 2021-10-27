/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:32 by caniseed          #+#    #+#             */
/*   Updated: 2021/10/27 18:10:18 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H
# define PHILO_H

# define SUCCESS 0
# define ERROR 1

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_philo
{
	int	tread; //??
	int	time_of_last_meal;
	int	id;
	int	number_of_forks;
	int	number_of_meals_eaten;
}				t_philo;

typedef struct s_arg
{
	int		number_of_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philo_must_eat; // number_of_meals
	t_philo	*philo;
}				t_arg;


#endif
