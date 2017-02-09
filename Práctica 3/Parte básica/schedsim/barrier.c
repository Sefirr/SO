#include "barrier.h"
#include "semaphore.h"
#include <errno.h>


#ifdef POSIX_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nthreads) {   
    return pthread_barrier_init(barrier,NULL,nthreads);
}

int sys_barrier_destroy(sys_barrier_t* barrier) {
    return pthread_barrier_destroy(barrier);
}

int sys_barrier_wait(sys_barrier_t *barrier) {
    return pthread_barrier_wait(barrier);
}

#else


/* Inicializacion de la barrera */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
   /* Inicializar campos de la estructura sys_barrier
        ... Completar ....
   */
   int err;
    
   if(!barrier)
      return EINVAL;
   if(nr_threads == 0)
      return EINVAL;
   
   err = pthread_mutex_init(&barrier->mutex,  NULL);
   if(err)
      return err;
  
   err = pthread_cond_init(&barrier->cond, NULL); 
   if(err) {
      pthread_mutex_destroy(&barrier->mutex);
      return err;
   }

   barrier->nr_threads_arrived = 0;
   barrier->max_threads = nr_threads;

   return 0;
}

/* Destruccion de los recursos de la barrera */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
   /* Destruir recursos de sincronizacion asociados a la barrera
        ... Completar ....
   */
   pthread_cond_destroy(&barrier->cond);
   pthread_mutex_destroy(&barrier->mutex);

   return 0;
}

/* Operacion de sincronizacion/espera en la barrera */
int sys_barrier_wait(sys_barrier_t *barrier)
{
    /* Idea general implementacion:
       - El hilo que llega a la barrera adquiere el mutex e
        incrementa de forma atomica el contador nr_threads_arrived.
       - Si aun faltan mas hilos por llegar a la barrera el hilo se bloquea en la V.cond.
       - Si es el ultimo hilo en llegar:
            1. resetea el estado de la barrera para el siguiente sys_barrier_wait() y
            2. despierta a todos los hilos bloqueados en la V. cond.
       - Liberar mutex antes de salir de la funcion

        ... Completar ....  
    */
    pthread_mutex_lock(&barrier->mutex);
    (barrier->nr_threads_arrived)++;
    if(barrier->nr_threads_arrived < barrier->max_threads) {
       pthread_cond_wait(&barrier->cond, &(barrier->mutex));
    } else {
       barrier->nr_threads_arrived = 0;
       pthread_cond_broadcast(&barrier->cond);
    }

    pthread_mutex_unlock(&barrier->mutex);

    return 0;
}

#endif /* POSIX_BARRIER */

#ifdef SEM_BARRIER

/* Inicializacion de la barrera */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
   /* Inicializar campos de la estructura sys_barrier
        ... Completar ....
   */
   int err;
    
   if(!barrier)
      return EINVAL;
   if(nr_threads == 0)
      return EINVAL;
   
   err = sem_init(&barrier->mtx,0,1);
   if(err)
      return err;
  
   err = sem_init(&barrier->queue,0,0);
   if(err) {
      sem_destroy(&barrier->mtx);
      return err;
   }

   barrier->nr_threads_arrived = 0;
   barrier->max_threads = nr_threads;

   return 0;
}

/* Destruccion de los recursos de la barrera */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
   /* Destruir recursos de sincronizacion asociados a la barrera
        ... Completar ....
   */
   sem_destroy(&barrier->mtx);
   sem_destroy(&barrier->queue);

   return 0;
}

/* Operacion de sincronizacion/espera en la barrera */
int sys_barrier_wait(sys_barrier_t *barrier)
{
    /* Idea general implementacion:
       - El hilo que llega a la barrera adquiere el mutex e
        incrementa de forma atomica el contador nr_threads_arrived.
       - Si aun faltan mas hilos por llegar a la barrera el hilo se bloquea en la cola.
       - Si es el ultimo hilo en llegar:
            1. despierta a todos los hilos bloqueados en la cola haciendo un for del número de hilos que han llegado a la cola y
            2. resetea el estado de la barrera para el siguiente sys_barrier_wait().
       - Liberar el semáforo antes de salir de la funcion (actúa como un mutex).

        ... Completar ....  
    */
    int i;
    sem_wait(&barrier->mtx);
    (barrier->nr_threads_arrived)++;
    if(barrier->nr_threads_arrived < barrier->max_threads) {
       sem_post(&barrier->mtx);
       sem_wait(&barrier->queue);
       sem_wait(&barrier->mtx);
    } else {
       for(i = 0; i < barrier->nr_threads_arrived; i++)
       		sem_post(&barrier->queue);
       barrier->nr_threads_arrived = 0;
    }

    sem_post(&barrier->mtx);

    return 0;
}

#else


/* Inicializacion de la barrera */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
   /* Inicializar campos de la estructura sys_barrier
        ... Completar ....
   */
   int err;
    
   if(!barrier)
      return EINVAL;
   if(nr_threads == 0)
      return EINVAL;
   
   err = pthread_mutex_init(&barrier->mutex,  NULL);
   if(err)
      return err;
  
   err = pthread_cond_init(&barrier->cond, NULL); 
   if(err) {
      pthread_mutex_destroy(&barrier->mutex);
      return err;
   }

   barrier->nr_threads_arrived = 0;
   barrier->max_threads = nr_threads;

   return 0;
}

/* Destruccion de los recursos de la barrera */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
   /* Destruir recursos de sincronizacion asociados a la barrera
        ... Completar ....
   */
   pthread_cond_destroy(&barrier->cond);
   pthread_mutex_destroy(&barrier->mutex);

   return 0;
}

/* Operacion de sincronizacion/espera en la barrera */
int sys_barrier_wait(sys_barrier_t *barrier)
{
    /* Idea general implementacion:
       - El hilo que llega a la barrera adquiere el mutex e
        incrementa de forma atomica el contador nr_threads_arrived.
       - Si aun faltan mas hilos por llegar a la barrera el hilo se bloquea en la V.cond.
       - Si es el ultimo hilo en llegar:
            1. resetea el estado de la barrera para el siguiente sys_barrier_wait() y
            2. despierta a todos los hilos bloqueados en la V. cond.
       - Liberar mutex antes de salir de la funcion

        ... Completar ....  
    */
    pthread_mutex_lock(&barrier->mutex);
    (barrier->nr_threads_arrived)++;
    if(barrier->nr_threads_arrived < barrier->max_threads) {
       pthread_cond_wait(&barrier->cond, &(barrier->mutex));
    } else {
       barrier->nr_threads_arrived = 0;
       pthread_cond_broadcast(&barrier->cond);
    }

    pthread_mutex_unlock(&barrier->mutex);

    return 0;
}

#endif
