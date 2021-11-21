/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:32 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/21 23:03:02 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define SUCCESS 0
# define ERROR 1
# define DEATH 2
# define NO_MEALS_LEFT 3
# define FINISHED_EATING 4
# define OK 5

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_philo
{
	unsigned long	time_of_last_meal;
	int				id;
	pthread_t		thread;
	int				right_fork;
	int				left_fork;
	int				meals_counter;
	struct s_arg	*data_main;
}				t_philo;

typedef struct s_arg
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_philo_must_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	unsigned long	time_from_start;
	t_philo			*philo;
}				t_arg;

//t_arg	*g_data;

int				ft_atoi(const char *str);
int				check_for_error(int argc, char **argv);
unsigned long	get_time(void);
void			data_init_2(t_arg *main_data);
t_arg			*data_init(char **argv, t_arg *main_data);
void			my_usleep(unsigned long wait_time);
void			print_message_2(int flag, int id, unsigned long time, t_arg *main_data);
void			print_message(int flag, int id, unsigned long time, t_arg *main_data);
void			*philo_actions(void *data, t_arg *main_data);
void			even_thread_create(t_arg *main_data);
void			odd_thread_create(t_arg *main_data);
int				waiter(t_arg *main_data);
void			mutex_init(t_arg *main_data);
void			join_thread(t_arg *main_data);
void			detach_thread(t_arg *main_data);
void			mutex_destroy_and_free(t_arg *main_data);

#endif
