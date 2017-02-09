#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_OF_PHILOSOPHERS 5

pthread_t philosophers[NO_OF_PHILOSOPHERS];
pthread_mutex_t tenedores[NO_OF_PHILOSOPHERS];


void init()
{
    int i;
    for(i=0; i<NO_OF_PHILOSOPHERS; i++)
        pthread_mutex_init(&tenedores[i],NULL);

    
}

void think(int i) {
    printf("Filósofo %d pensando... \n" , i);
        sleep(random() % 10);
    printf("Filósofo %d teminó de pensar!!! \n" , i);

}

void eat(int i) {
    printf("Filósofo %d comiendo... \n" , i);
     sleep(random() % 5);
    printf("Filósofo %d teminó de comer!!! \n" , i);

}

void toSleep(int i) {
    printf("Filósofo %d durmiendo... \n" , i);
    sleep(random() % 10);
    printf("Filósofo %d teminó de dormir!!! \n" , i);
    
}

void* philosopher(void* i)
{
    int nFilosofo = (int)i;
    int derecha = nFilosofo;
    int izquierda = (nFilosofo - 1 == -1) ? NO_OF_PHILOSOPHERS - 1 : (nFilosofo - 1);
    while(1)
    {
        
        think(nFilosofo);
        
        /// INTENTAR COGER AMBOS TENEDORES (derecha e izquierda)
        pthread_mutex_lock(&tenedores[derecha]);
	//lock(tenedores[(i+1)%NO_OF_PHILOSOPHERS]);
	if (philosophers[derecha] > philosophers[izquierda]) {
    		pthread_mutex_lock(&tenedores[izquierda]);	
 	} else {
    		pthread_mutex_unlock(&tenedores[derecha]);
    		pthread_mutex_lock(&tenedores[izquierda]);
    		pthread_mutex_lock(&tenedores[derecha]);    
 	}
        
	eat(nFilosofo);
        
        // DEJAR AMBOS TENEDORES
	pthread_mutex_unlock(&tenedores[derecha]);
	pthread_mutex_unlock(&tenedores[izquierda]);
        
        toSleep(nFilosofo);
   }

}

int main()
{
    init();
    unsigned long i;
    for(i=0; i<NO_OF_PHILOSOPHERS; i++)
        pthread_create( &philosophers[i], NULL, philosopher, (void*)i);
    
    for(i=0; i<NO_OF_PHILOSOPHERS; i++)
        pthread_join(philosophers[i],NULL);
    return 0;
} 
