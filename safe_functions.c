/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllarena <jllarena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:05:59 by jllarena          #+#    #+#             */
/*   Updated: 2024/05/14 19:36:36 by jllarena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (ret == NULL)
		error_exit("error: malloc");
	return (ret);
}
static void	pthread_error(int status, t_opcode opcode);
static void	mutex_error(int status, t_opcode opcode);
/*
En resumen, mientras que un mutex es un mecanismo específico utilizado para la
sincronización de hilos al controlar el acceso a recursos compartidos, pthread
es una biblioteca que proporciona funciones para la creación y administración de
hilos, incluyendo el uso de mutexes para la sincronización. Los mutexes son solo
uno de los muchos componentes de la API 
pthreads que facilitan la programación de sistemas concurrentes y paralelos.
*/
/*
toda las funciones devolveran un 0 si es correcto y cualquier numero si no.
*/

void	safe_mutex(t_mtx *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (UNLOCK == opcode)
		mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (INIT == opcode)
		mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		error_exit("error: wrong opcode for mutex");
}
/*
EINVAL... son codigos de error pertencientes a la liberia errno.h
estoss codigos salen en errors en ej: man pthread_mutex_init
*/

static void	mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (((EINVAL == status) && (opcode == LOCK)) || (opcode == UNLOCK))
		error_exit("the value is invalid");
	else if (EDEADLK == status)
		error_exit("A deadlock would occur if the thread "
			"blocked waiting for mutex.");
	else if (EPERM == status)
		error_exit("The current thread does not hold a lock on mutex.");
	else if (ENOMEM == status)
		error_exit("The process cannot allocate enough memory"
			" to create another mutex.");
	else if (EBUSY == status)
		error_exit("Mutex is locked");
}
/*
pthread_create:
Un puntero a una variable de tipo pthread_t, que será utilizada para
almacenar el identificador del nuevo hilo.
Un puntero a los atributos del hilo (puede ser NULL si deseas usar los
atributos predeterminados).
Una función que será ejecutada por el nuevo hilo.
Un puntero que será pasado como argumento a la función del hilo.
*/

void	safe_thread(pthread_t *thread, void *(*foo)(void *),
		void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		pthread_error(pthread_create(thread, NULL, foo, data), opcode);
	else if (JOIN == opcode)
		pthread_error(pthread_join(*thread, NULL), opcode);
	else if (DETACH == opcode)
		pthread_error(pthread_detach(*thread), opcode);
	else
		error_exit("Wrong opcode for thread_handle:"
			" use <CREATE> <JOIN> <DETACH>");
}

/*
codigos de error que salen en ej: man pthread_create
*/
static void	pthread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EAGAIN == status)
		error_exit("No resources to create another thread");
	else if (EPERM == status)
		error_exit("The caller does not have appropriate permission\n");
	else if (EINVAL == status && CREATE == opcode)
		error_exit("The value specified by attr is invalid.");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		error_exit("The value specified by thread is not joinable\n");
	else if (ESRCH == status)
		error_exit("No thread could be found corresponding to that"
			"specified by the given thread ID, thread.");
	else if (EDEADLK == status)
		error_exit("A deadlock was detected or the value of"
			"thread specifies the calling thread.");
}
