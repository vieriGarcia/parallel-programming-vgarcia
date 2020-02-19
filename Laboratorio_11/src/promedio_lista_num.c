#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * Hello OpenMP World prints the number of threads and the current thread id
 * Compilacion: gcc -o Laboratorio_11 -fopenmp  promedio_lista_num.c
 * Ejecución:./Laboratorio_11
 */
int main(int argc, char *argv[]) {

	int numThreads, tid,n;
	int *x;
	float prom=0;
	float suma=0;
	double local_n;
	printf("Ingrese el número de hilos:");
	scanf("%d",&numThreads);
	omp_set_num_threads(numThreads);
	printf("Ingrese la dimensión:");
	scanf("%d",&n);
	if(n%numThreads==0){
		local_n=n/numThreads;
		//Asignación de memoria
		x=malloc(n*sizeof(int));
		//Lectura de datos
		for(int i=0;i<n;i++){
			printf("x[%d]=",i+1);
			scanf("%d",&x[i]);
		}

		printf("\n");
		//Paralelización del cálculo de promedio
		#pragma omp parallel private(tid)
		{
		  tid = omp_get_thread_num();
		  int u=0;
		   for(int i=0;i<local_n;i++){
			   u=tid+i*numThreads;
			   suma=suma+x[u];
		   }
		 }
		printf("\n");
		prom=suma/n;
		printf("La suma es %.2f\n",suma);
		printf("\nEl promedio es %.2f\n",prom);

	}else{
		printf("\nLa dimensión debe ser divisible por el número de hilos\n");
	}




	return 0;
}

