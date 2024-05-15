/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllarena <jllarena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:26:15 by jllarena          #+#    #+#             */
/*   Updated: 2024/05/15 17:07:11 by jllarena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>
# include <errno.h>

# ifndef DEBUG_MODE
#  define DEBUG_MODE 0
# endif

/*
 PHILO MAX es 200
*/
# ifndef PHILO_MAX
#  define PHILO_MAX 200 
# endif

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}			t_philo_status;

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_opcode;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}	t_time_code;

/*estructura para el mutex*/

typedef struct s_table	t_table;
typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}	t_fork;

/*
estrucutra para cada filosofo
*/
typedef struct s_philo
{
	int			id;
	long		meals_counter;
	bool		full; //booleano si ha supperado el maximo de comidas 
	long		last_meal_time; //tiempo desde la ultima comida
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id; // un philosofo es un thread
	t_mtx		philo_mutex;
	t_table		*table;
}	t_philo;

/* ./philo 5 800 200 200 [5]  */

struct s_table
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		nbr_limit_meals; //limite de comidas por ejemplo [5]
	long		start_simulation;
	bool		end_simulation; //booleano un philo muere o todos estan llenos
	bool		all_threads_ready; //syncro filosophers
	long		threads_running_nbr;
	pthread_t	monitor; //hilo para monitorear los filosofos
	t_mtx		table_mutex; //mutex al table para evitar carreras de hilos
	t_mtx		write_mutex;
	t_fork		*forks;//array forks
	t_philo		*philos; //array philosophos
};

void				error_exit(const char *error);
void				parse_input(t_table *table, char **argv);
void				parse_input(t_table *table, char **argv);
void				*safe_malloc(size_t bytes);
void				safe_mutex(t_mtx *mutex, t_opcode opcode);
void				safe_thread(pthread_t *thread, void *(*foo)(void *),
						void *data, t_opcode opcode);
void				data_init(t_table *table);
void				set_bool(t_mtx	*mutex, bool *dest, bool value);
bool				get_bool(t_mtx *mutex, bool *value);
long				get_long(t_mtx *mutex, long *value);
void				set_long(t_mtx *mutex, long *dest, long value);
bool				simulation_finished(t_table *table);
void				precise_usleep(long usec, t_table *table);
void				*monitor_dinner(void *data);
void				wait_all_threads(t_table *table);
void				set_long(t_mtx *mutex, long *dest, long value);
void				increase_long(t_mtx *mutex, long *value);
void				de_synchronize_philos(t_philo *philo);
void				write_status(t_philo_status status, t_philo *philo,
						bool debug);
long				gettime(int time_code);
void				dinner_start(t_table *table);
void				clean(t_table *table);
bool				all_threads_running(t_mtx *mutex, long *threads,
						long philo_nbr);
void				thinking(t_philo *philo, bool pre_simulation);

#endif