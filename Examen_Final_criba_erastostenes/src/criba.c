/*
 ============================================================================
 Name        : criba.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 20
int *x=NULL;
int *y=NULL;

void leer_n(int *n){
	printf("Ingrese un número:");
	scanf("%d",n);
}
void imprimir_array_except(int x[], int n,int exception){
	printf("Los números primos menores a %d son:",n);
	for(int i=0;i<n;i++){
		if(x[i]!=exception){
			printf("%d,",x[i]);
		}
	}
	printf("\b\b\n");
}
void crear_array(int n, int **x){
	*x=malloc(n*sizeof(int));
}
void criba_secuencial(int x[], int n){
	int j=0;
	//Inicializar array
	for(int i=0;i<n;i++){
	  if(((i+1)%2!=0 || i==1 )&& i!=0){
		  x[i]=i+1;
	  }else{
		  x[i]=-1;
	  }
	}
	//Obtención de números primos
	while(j<n){
		if(x[j]!=-1){
			for(int i=0;i<n;i++){
				if(x[i]!=-1 && i!=j && x[j]%x[i]==0){
				x[j]=-1;
				break;
				}
			}
		}
	   j++;
	}
	//Imprimir resultado
	imprimir_array_except(x,n,-1);
}
void criba_paralela(int x[], int n){
//Inicializar valor
#pragma omp parallel for num_threads(NUM_THREADS )
	for(int i=0;i<n;i++){

		if(((i+1)%2!=0 || i==1 )&& i!=0){
		  x[i]=i+1;
		}else{
		  x[i]=-1;
		}
	}
//Obtención de números primos
#pragma omp parallel for num_threads(NUM_THREADS)
	for(int i=0;i<n;i++){
		if(x[i]!=-1){
			for(int u=0;u<n;u++){
			 if(x[i]!=-1 && x[u]!=-1 && u!=i && x[i]%x[u]==0){
			   #pragma omp critical
			    x[i]=-1;
			  }
	        }
		}
	}
	//Imprimir resultado
	imprimir_array_except(x,n,-1);
}
int main (int argc, char *argv[]) {

 int n=0;
 double start,finish,time;
 leer_n(&n);
 // Criba paralela
  crear_array(n,&x);
  start=omp_get_wtime();
  criba_paralela(x, n);
  finish=omp_get_wtime();
  time=finish-start;
  printf("Tiempo de ejecución paralelo  : %f\n",time);
 //Criba secuencial
 crear_array(n,&y);
 start=omp_get_wtime();
 criba_secuencial(y, n);
 time=finish-start;
 finish=omp_get_wtime();
 time=finish-start;
  printf("Tiempo de ejecución secuencial: %f\n",time);


 return 0;
}


