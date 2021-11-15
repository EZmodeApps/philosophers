/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/15 19:49:53 by caniseed         ###   ########.fr       */
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

unsigned long get_time(void)
{
	unsigned long result;
	struct timeval time;

	gettimeofday(&time, NULL);
	result = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return(result);
}

t_arg	*data_init(char **argv)
{
	//t_arg	*g_data;

	g_data = malloc(sizeof(t_arg));
	if (!g_data)
		exit(ERROR);
	g_data->number_of_philo = ft_atoi(argv[1]);
	g_data->time_to_die = ft_atoi(argv[2]);
	g_data->time_to_eat = ft_atoi(argv[3]);
	g_data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		g_data->number_of_times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		g_data->number_of_times_each_philo_must_eat = 0;
	g_data->time_start = get_time();
	g_data->forks = malloc(sizeof(pthread_mutex_t) * g_data->number_of_philo);
	if (!g_data->forks)
	{
		free(g_data);
		exit(ERROR);
	}
	pthread_mutex_init(&g_data->print_mutex, NULL);
	g_data->philo = malloc(sizeof(t_philo) * g_data->number_of_philo);
	if (!g_data->philo)
	{
		free(g_data->forks);
		free(g_data);
		exit(ERROR);
	}
	return (g_data);
}

void my_usleep(unsigned wait_time)
{
	unsigned long check;
	//unsigned long stop;

	check = get_time();
	//stop = wait_time/1000;
	while ((get_time() - check) < wait_time)
		usleep(1000);
}

//void	my_sleep(unsigned long time_wait, unsigned long start_time)
//{
//	unsigned long n;
//
//	n = get_time() - start_time;
//	while (n < time_wait)
//	{
//		n = get_time() - start_time;
//		usleep(1000);
//	}
//}

void print_message(int flag, int id, unsigned long time)
{
	pthread_mutex_lock(&g_data->print_mutex);
	if (flag == 1)
	{
		printf("%ld %d has taken a fork\n", get_time() - time, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return ;
	}
	if (flag == 2)
	{
		printf("%ld %d is eating\n", get_time() - time, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return;
	}
	if (flag == 3)
	{
		printf("%ld %d is sleeping\n", get_time() - time, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return;
	}
	if (flag == 4)
	{
		printf("%ld %d is thinking\n", get_time() - time, id);
		pthread_mutex_unlock(&g_data->print_mutex);
		return;
	}
	if (flag == 5)
	{
		printf("%ld %d died\n", get_time() - time, id);
		return ;
	}

}

void	*philo_actions(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	philo->time_from_start = get_time();
	philo->time_of_last_meal = get_time();
	philo->time_without_meal = get_time() - philo->time_from_start;
		philo->left_fork = philo->id - 1;
	if (philo->id == g_data->number_of_philo)
		philo->right_fork = 0;
	else
		philo->right_fork = philo->id;
	while (1)
	{
		pthread_mutex_lock(&g_data->forks[philo->right_fork]);
		print_message(1, philo->id, philo->time_from_start);
		pthread_mutex_lock(&g_data->forks[philo->left_fork]);
		print_message(1, philo->id, philo->time_from_start);
		print_message(2, philo->id, philo->time_from_start);
		philo->is_eating_now = 1;
		philo->number_of_meals_eaten++;
		my_usleep(g_data->time_to_eat);
		philo->time_of_last_meal = get_time();
		pthread_mutex_unlock(&g_data->forks[philo->left_fork]);
		pthread_mutex_unlock(&g_data->forks[philo->right_fork]);
		philo->is_eating_now = 0;
		print_message(3, philo->id, philo->time_from_start);
		my_usleep(g_data->time_to_sleep);
		print_message(4, philo->id, philo->time_from_start);
		//usleep(1000);
	}
}

void even_tread_create(void)
{
	int i;

	i = 1;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		pthread_create(&g_data->philo[i].thread, NULL, philo_actions, (void *)&g_data->philo[i]);
		i += 2;
	}
}

void odd_tread_create(void)
{
	int i;

	i = 0;
	while (i < g_data->number_of_philo)
	{
		g_data->philo[i].id = i + 1;
		pthread_create(&g_data->philo[i].thread, NULL, philo_actions, (void *)&g_data->philo[i]);
		i += 2;
	}
}

//int *waiter_actions(void *data)
//{
//	int i;
//
//	(void)data;
//	i = 0;
//	while (i < g_data->number_of_philo)
//	{
//		g_data->philo[i].time_without_meal = get_time() - g_data->philo[i].time_of_last_meal;
//		if (get_time() - g_data->philo[i].time_without_meal > g_data->time_to_eat)
//		{
//			printf("%ld %d died\n", get_time() - g_data->philo[i].time_from_start, g_data->philo[i].id);
//			pthread_mutex_lock(&g_monitor);
////			return (int*)(1);
//		exit(ERROR);
//		}
//		if (g_data->number_of_times_each_philo_must_eat > 0 && g_data->philo[i].number_of_meals_eaten == g_data->number_of_times_each_philo_must_eat)
//		{
//			pthread_mutex_lock(&g_monitor);
////			return (int*)(1);
//		//	exit(ERROR);
//		}
//		if (i == g_data->number_of_philo)
//			i = 0;
//		i++;
//	}
//	return (0);
//}

int waaiter(void)
{
	int i;

	i = 1;
	while (1)
	{
		if (i == g_data->number_of_philo)
			i = 1;
		while (i < g_data->number_of_philo)
		{
//			printf("%ld\n", get_time() - g_data->philo[i].time_of_last_meal);
//			printf("%ld\n", g_data->philo[i].time_of_last_meal);
//			printf("%ld\n", g_data->philo[i].time_from_start);
			if (g_data->philo[i].time_of_last_meal - g_data->philo[i].time_from_start > g_data->time_to_eat)
			{
				print_message(5, g_data->philo[i].id, g_data->philo[i].time_from_start);
//				exit(ERROR);
				return(2);
			}
			if (g_data->number_of_times_each_philo_must_eat > 0 &&
				g_data->philo[i].number_of_meals_eaten == g_data->number_of_times_each_philo_must_eat)
			{
				return (3);
				exit(SUCCESS);
			}
			i++;
		}
	}
}

int	main(int argc, char **argv)
{
	int i;
	int check;
	pthread_t waiter;

	i = 0;
	check_for_error(argc, argv);
	g_data = malloc(sizeof(t_arg));
	data_init(argv);
	while (i < g_data->number_of_philo)
	{
		pthread_mutex_init(&g_data->forks[i], NULL);
		i++;
	}
	even_tread_create();
	usleep(1000);
	odd_tread_create();
//	pthread_create(&waiter, NULL, (void *)waiter_actions, (void *)i);
	check = waaiter();
	i = 0;
//	if (number_of_times_each_philo_must_eat > 0) //все наелись number_of_times_each_philo_must_eat == 0
//	{
//		while (i < g_data->number_of_philo) {
//			pthread_join(g_data->philo[i].thread, NULL);
//			i++;
//		}
//	}
	if (check == 2)
	{
		while (i < g_data->number_of_philo)
		{
			pthread_detach(g_data->philo[i].thread);
			i++;
		}
		return (0);
	}
//	pthread_join(waiter, NULL);
	return (SUCCESS);
}