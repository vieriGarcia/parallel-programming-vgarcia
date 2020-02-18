/*
 ============================================================================
 Name        : openMP_prueba.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int thread_count,local_n;
double total_area=0,a,b,n,h;
double f(double x){
	return x*x;
}
void *Rutina(void* rank) {//Cálculo de las áreas locales de cada trapecio
 long my_rank = (long) rank;
 double local_a,local_b,local_area=0;
 local_a= a + my_rank*local_n*h;
 local_b=local_a + local_n*h;
 local_area=local_n*h*(f(local_a)+f(local_b))/2;
 double local_x;
   int i;

   local_area = (f(local_a) + f(local_b))/2;
   for (i = 1; i <= local_n-1; i++) {
      local_x = local_a + i*h;
      local_area += f(local_x);
   }
   local_area = local_area*h;
 total_area+=local_area;
 return NULL;
}
int main (int argc, char *argv[]) {

 long thread;
 int divisible=0;
 pthread_t* thread_handles;
 thread_count = strtol(argv[1], NULL, 10);
 thread_handles = malloc(thread_count * sizeof(pthread_t));

 //Obtener a
  printf("a=");
  scanf("%lf",&a);
  //Obtener b
  printf("b=");
  scanf("%lf",&b);
  //Obtener n
  printf("n=");
  scanf("%lf",&n);
  // Identificar si n es divisible por el número de hilos
  if((int)n%thread_count==0){
	  h=(b-a)/n;
	  local_n=n/thread_count;
	  divisible=1;
  }
  // En caso sea divisible
  if(divisible==1){
	//Creación de hilos y ejecución de Rutina
	 for(thread = 0; thread < thread_count; thread++)
		 pthread_create(&thread_handles[thread], NULL,Rutina, (void*) thread);
	 for (thread = 0; thread < thread_count; thread++)
		 pthread_join(thread_handles[thread], NULL);
	 free(thread_handles);
	  printf("El área total es %.2f\n",total_area);
  }else{
	  printf("El número %.2f no es múltiplo del número de hilos %d\n",n,thread_count);
  }


 return 0;
}


