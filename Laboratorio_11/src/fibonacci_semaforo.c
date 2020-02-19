/*
 ============================================================================
 Name        : Laboratorio_11.c
 Author      : gg
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 Compilación: gcc -g -Wall -o Laboratorio_11 fibonacci_semaforo.c -lpthread
 Ejecución  : ./Laboratorio_11 <numero_hilos>
 ============================================================================
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
int thread_count,x;
int *f;
sem_t* semaphores;

void *Rutina(void* rank) {
  long my_rank = (long) rank;
  int count=my_rank+3;
  long dest = (my_rank + 1) % thread_count;

  sem_wait(&semaphores[my_rank]);

  f[count]=f[count-1]+f[count-2];
  //printf("rank %ld - f %d\n",my_rank,f[count]);
  sem_post(&semaphores[dest]);


 return NULL;
}
int main (int argc, char *argv[]) {

 long thread;
 int divisible=0;
 pthread_t* thread_handles;
 thread_count = strtol(argv[1], NULL, 10);
 thread_handles = malloc(thread_count * sizeof(pthread_t));
 semaphores = malloc(thread_count * sizeof(sem_t));

 //Obtener x
  printf("Ingrese un número que a lo sumo exceda en 3 a la cantidad de hilos %d y mayor a esta\nx=",thread_count);
  scanf("%d",&x);
  f= malloc((x)*sizeof(int));
  f[0]=1;
  f[1]=1;
  f[2]=1;
  // Identificar si x es divisible por el número de hilos
  if(x<=thread_count+3){
	  divisible=1;
  }
  // En caso sea divisible
  if(divisible==1){
	  //En caso x sea distinto de 0 o 1

	  if(x!=0 && x!=1 && x!=2){
		  int s=0;
		  //Creación de hilos y ejecución de Rutina
		  for(thread = 0; thread < thread_count; thread++){
			  if(thread==0)s=1;
			  sem_init(&semaphores[thread], 0, s);
			  s=0;
		  }


	  	  for(thread = 0; thread < thread_count; thread++)
	  	  	 pthread_create(&thread_handles[thread], NULL,Rutina, (void*) thread);

	  	  for (thread = 0; thread < thread_count; thread++)
	  	   pthread_join(thread_handles[thread], NULL);

	  	  for (thread = 0; thread < thread_count; thread++)
	  	   sem_destroy(&semaphores[thread]);

	  	  free(thread_handles);
	  	  free(semaphores);
	  }
	  printf("Fibonacci Nº%d=%d\n",x,f[x-1]);
	  	  for(int i=0;i<x;i++){
	  		  printf("%d,",f[i]);
	  	  }
	  printf("\n");
  }else{
	  printf(" %d excede en más de 3 al número de hilos \n",x);
  }


 return 0;
}



