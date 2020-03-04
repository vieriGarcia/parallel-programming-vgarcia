/*
 ============================================================================
 Name        : estudiando.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_HILOS 5
int contador=0;
int x[5]={1,2,3,4,5};
pthread_mutex_t mutex;
// sem_t * semaforos;
/*void* rutina_semaforo (void* rank){
	int i=(int)rank;
	long orig=(long)rank;
	long dest=orig+1 ;
	if(i==0){
		printf("x[%d]=%d\n",i,x[i]);
		sem_post(&semaforos[dest]);
	}else{
	    sem_wait(&semaforos[orig]);
	    printf("x[%d]=%d\n",i,x[i]);
	    sem_post(&semaforos[dest]);
	}
	return NULL;
}*/
void* rutina_mutex(){
  pthread_mutex_unlock(&mutex);
  printf("x[%d]=%d\n",contador,x[contador]);
  contador++;
  pthread_mutex_unlock(&mutex);
  return NULL;
}
int main (int argc, char *argv[]) {
 pthread_t* hilos;
 hilos=malloc(NUM_HILOS*sizeof(pthread_t));
 pthread_mutex_init(&mutex,NULL);
 /*semaforos=malloc(NUM_HILOS*sizeof(sem_t));
 for(int i=0;i<NUM_HILOS;i++){
 	 sem_init(&semaforos[i],0,0);
  }*/
 for(long i=0;i<NUM_HILOS;i++){
	 //pthread_create(&hilos[i],NULL,rutina,(void*)i);
	 pthread_create(&hilos[i],NULL,rutina_mutex,NULL);
 }
 printf("Proceso principal...\n");
 for(int i=0;i<NUM_HILOS;i++){
 	 pthread_join(hilos[i],NULL);
  }
 /*for(int i=0;i<NUM_HILOS;i++){
  	 sem_destroy(&semaforos[i]);
   }*/
 free(hilos);
 return 0;
}


