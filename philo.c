/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:08:24 by caniseed          #+#    #+#             */
/*   Updated: 2021/10/27 23:43:56 by caniseed         ###   ########.fr       */
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

t_arg	data_init(void)
{
	t_arg *data;

	data = malloc(sizeof(t_arg));
	if (!data)
		exit(ERROR);
	data->number_of_philo = 0;
	data->time_to_die = 0;
	data->time_to_eat = 0;
	data->time_to_sleep = 0;
	data->time_to_sleep = 0;
	data->number_of_times_each_philo_must_eat = 0;
	data->philo = malloc(sizeof(t_philo));
	if (!data->philo)
	{
		free(data);
		exit(ERROR);
	}
	data->philo->tread = 0;
	data->philo->time_of_last_meal = 0;
	data->philo->id = 0;
	data->philo->number_of_forks = 0;
	data->philo->number_of_meals_eaten = 0;
}

int	main(int argc, char **argv)
{
	t_arg	*data; //????

	check_for_error(argc, argv);
	data = data_init();
	return (SUCCESS);
}