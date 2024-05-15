/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllarena <jllarena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:41 by jllarena          #+#    #+#             */
/*   Updated: 2024/05/14 18:42:12 by jllarena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
la función wait_all_threads espera activamente hasta que todos los
hilos estén listos, verificando continuamente el valor de all_threads_ready
mediante la función get_bool. La función get_bool garantiza que la lectura del
valor booleano sea segura en un entorno multihilo mediante el bloqueo y
desbloqueo de un mutex.
Una vez que todos los hilos están listos (all_threads_ready == true), la función
get_bool devolverá true. Esto hará que la condición del bucle se vuelva falsa y
el bucle se detenga. Por lo tanto, el hilo que llama a esta función saldrá del
bucle y continuará con su ejecución.
*/
void	wait_all_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		;
}

/*
incrementa el valor de hilos, asi puedo monitorear y controlar la simulacion
gracias al contador de hilos
*/
void	increase_long(t_mtx *mutex, long *value)
{
	safe_mutex(mutex, LOCK);
	(*value)++;
	safe_mutex(mutex, UNLOCK);
}

/*
 * Monitor waits all threads are running the 
 * simulation before searching deaths
*/
bool	all_threads_running(t_mtx *mutex, long *threads,
		long philo_nbr)
{
	bool	ret;

	ret = false;
	safe_mutex(mutex, LOCK);
	if (*threads == philo_nbr)
		ret = true;
	safe_mutex(mutex, UNLOCK);
	return (ret);
}

/*
si el numero de philos y y de id es par, van a esperar 30 milisegundos,
si es impar llama a la funcion thinking
*/
void	de_synchronize_philos(t_philo *philo)
{
	if (philo->table->philo_nbr % 2 == 0)
	{
		if (philo->id % 2 == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			thinking(philo, true);
	}
}
