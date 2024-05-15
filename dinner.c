/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllarena <jllarena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:32:31 by jllarena          #+#    #+#             */
/*   Updated: 2024/05/14 18:03:50 by jllarena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
si pre_simulation es falso esribe THINKING, se calcula los tiempos de think
y si es negativo se pone en 0.
*/
void	thinking(t_philo *philo, bool pre_simulation)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_simulation)
		write_status(THINKING, philo, DEBUG_MODE);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	t_think = (t_eat * 2) - t_sleep;
	if (t_think < 0)
		t_think = 0;
	precise_usleep(t_think * 0.42, philo->table);
}

/*
Espera que los hilos esten sincronizados y actualiza el tiempo de la ultima
comida
La función lone_philo maneja el caso especial de un filósofo que está 
solo en la mesa. Realiza las siguientes acciones:

Inicializa el filósofo.
-Espera a que todos los hilos estén listos.
-Actualiza el tiempo de la última comida.
-Incrementa el número de hilos en ejecución(seguramente, es decir,
bloquea el mutex, lo incremmenta y lo desbloquea).
-Simula la toma del primer tenedor (solo para propósitos de salida).
-Entra en un bucle donde espera hasta que la simulación termine,
durmiendo periódicamente para no consumir demasiados recursos de la CPU.
-Este bucle se rompe cuando el monitor de la simulación detecta que el
filósofo ha muerto, momento en el que la función finaliza.
*/
void	*lone_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_finished(philo->table))
		precise_usleep(200, philo->table);
	return (NULL);
}

/*
bloqueo los mutex de los tenedores y escribo el estado de estos, es decir, que 
han sido cogidos. Actalizo la hora de la ultima comida y contabilizo la
comida actual. escribo el estado y simula el tiempo de comida.
si he alcanzado el limite de comidas pongo el booleano de full en true.
Despues desbloqueo los mutex de los tenedores para que lo puedan usar otros
filosofos(hilos).
*/
static void	eat(t_philo *philo)
{
	safe_mutex(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	safe_mutex(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	philo->meals_counter++;
	write_status(EATING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter == philo->table->nbr_limit_meals)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex(&philo->first_fork->fork, UNLOCK);
	safe_mutex(&philo->second_fork->fork, UNLOCK);
}

/*
se esera a que todos los hilos esten listos y se actualiza la hora de la
ultima comida, se increment el numero de hilos y se desincroniza los philos,
es decir, los philos pares con los impares.
Se inici el bucle mientas no haya termiando y si philo->full sale sin hacer
nada porque inidicaria ha llegado al limite de comidas.
Se llama a la funcion eat y despues una vez haya termiando se escribe el status 
de sleeping y se espera el tiempo de sueño establecido por time_to_sleep llamando
a precise_usleep.
Despues de dormir llama a la funcionde thinking
*/
static void	*dinner_simulation(void *data)
{
	t_philo		*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	de_synchronize_philos(philo);
	while (!simulation_finished(philo->table))
	{
		if (get_bool(&philo->philo_mutex, &philo->full))
			break ;
		eat(philo);
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		thinking(philo, false);
	}
	return (NULL);
}

/*
creo un hilo y llamo a la funcion lone_philo en casa de haber solo 1 philo,
si hay mas, creo un hilo para cada filososo y llamo a dinner_simulation.
Establece el tiempo de inicio de la simulación y marca a todos los hilos como
listos.
Espera a que todos los hilos terminen y luego marca el final de la simulación.
Espera a que el monitor termine también.

*/
void	dinner_start(t_table *table)
{
	int			i;

	i = -1;
	if (0 == table->nbr_limit_meals)
		return ;
	else if (1 == table->philo_nbr)
		safe_thread(&table->philos[0].thread_id, lone_philo,
			&table->philos[0], CREATE);
	else
		while (++i < table->philo_nbr)
			safe_thread(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	safe_thread(&table->monitor, monitor_dinner, table, CREATE);
	table->start_simulation = gettime(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		safe_thread(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread(&table->monitor, NULL, NULL, JOIN);
}
