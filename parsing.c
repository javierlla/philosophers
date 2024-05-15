/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllarena <jllarena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 17:54:22 by jllarena          #+#    #+#             */
/*   Updated: 2024/05/15 17:03:48 by jllarena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static const char	*valid_input(const char *str)
{
	int			len;
	const char	*number;
	int			i;

	i = 0;
	len = 0;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		error_exit("error: negative number");
	if (!is_digit(str[i]))
		error_exit("error: is not a number");
	number = str;
	while (is_digit(str[i++]))
		len++;
	if (len > 10)
		error_exit("the number is too long");
	return (number);
}

static long	ft_atoi(const char *str)
{
	long	num;
	int		i;

	i = 0;
	num = 0;
	str = valid_input(str);
	while (is_digit(str[i]))
		num = (num * 10) + (str[i++] - '0');
	if (num > INT_MAX)
		error_exit("the number is too long");
	return (num);
}

void	parse_input(t_table *table, char **argv)
{
	table->philo_nbr = ft_atoi(argv[1]);
	if (table->philo_nbr > PHILO_MAX)
	{
		printf("Max philos are %d\n", PHILO_MAX);
		exit(EXIT_FAILURE);
	}
	table->time_to_die = ft_atoi(argv[2]) * 1e3;
	table->time_to_eat = ft_atoi(argv[3]) * 1e3;
	table->time_to_sleep = ft_atoi(argv[4]) * 1e3;
	if (table->time_to_die < 6e4
		|| table->time_to_eat < 6e4
		|| table->time_to_sleep < 6e4)
		error_exit("use timestamps major than 60ms");
	if (argv[5])
	{
		table->nbr_limit_meals = ft_atoi(argv[5]);
	}
	else
		table->nbr_limit_meals = -1;
}
