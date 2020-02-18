/*
 ============================================================================
 Name        : openMP_prueba.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 Compilación: gcc -g -Wall -o Laboratorio_07 factorial.c -lpthread
 Ejecución  : ./ Laboratorio_07 <numero_hilos>

 ============================================================================
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int thread_count,factorial=1,local_x,x;
void *Rutina(void* rank) {
 long my_rank = (long) rank;
 int local_fac=1,u=0;
 for(int i=0;i<local_x;i++){
	 u=1+my_rank+i*thread_count;
	local_fac=local_fac*(u);
 }
 factorial=factorial*local_fac;
 return NULL;
}
int main (int argc, char *argv[]) {

 long thread;
 int divisible=0;
 pthread_t* thread_handles;
 thread_count = strtol(argv[1], NULL, 10);
 thread_handles = malloc(thread_count * sizeof(pthread_t));

 //Obtener x
  printf("x=");
  scanf("%d",&x);
  // Identificar si x es divisible por el número de hilos
  if(x==0||x%thread_count==0){
	  local_x=x/thread_count;
	  divisible=1;
  }
  // En caso sea divisible
  if(divisible==1){
	  //En caso x sea distinto de 0 o 1
	  if(x!=0 && x!=1){
		  //Creación de hilos y ejecución de Rutina
	  	  for(thread = 0; thread < thread_count; thread++)
	  	  	 pthread_create(&thread_handles[thread], NULL,Rutina, (void*) thread);
	  	  //
	  	   for (thread = 0; thread < thread_count; thread++)
	  	   pthread_join(thread_handles[thread], NULL);

	  	   free(thread_handles);
	    }
	  printf("%d!=%d\n",x,factorial);
  }else{
	  printf("El número %d no es múltiplo del número de hilos %d\n",x,thread_count);
  }


 return 0;
}


