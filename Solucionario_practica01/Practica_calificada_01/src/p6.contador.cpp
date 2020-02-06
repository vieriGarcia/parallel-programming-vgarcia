/*
 ============================================================================
 Name        : factorial.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 
#include "mpi.h" 
#include <iostream>
using namespace std;
void Read_n(int* n_p);
void Allocate_vectors(int** x_pp, int n);
void Read_vector(int a[], int n, char vec_name[]);
void Print_vector(int b[], int n, char title[]);
 
int main(int argc, char *argv[]) {
	int n,rank, size, local_total=0,total=0,local_n,x;
	int *A;
	int *local_A;
	bool printResult=true;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(comm,&rank);
	MPI_Comm_size(comm,&size);

	if(rank==0){
		Read_n(&n);
		Allocate_vectors(&A,n);
		Read_vector(A, n,"Vector A");
		Print_vector(A,n,"Vector A");
	}
	MPI_Bcast(&n,1,MPI_INT,0,comm);
	if(n%size==0){
		local_n=n/size;
		local_A=new int[local_n];
		MPI_Scatter(A,local_n,MPI_INT,local_A,local_n,MPI_INT,0,comm);
		if(rank==0){
				printf("Ingrese un número:");
				cin>>x;
		}
		MPI_Bcast(&x,1,MPI_INT,0,comm);
		for(int i=0;i<local_n;i++){
			if(local_A[i]==x){
				local_total++;
			}
		}
		MPI_Reduce(&local_total,&total,1,MPI_INT,MPI_SUM,0,comm);
	}else{
		printResult=false;
	}
	if(rank==0 ){
		if(!printResult){
			printf("número de procesos \"%d\" debe ser multiplo de la cantidad de elementos del arreglo\n",size);
		}else{
			printf("El número %d se repite %d veces\n",x,total);
		}
	}
	MPI::Finalize();
	return 0;
}
void Read_n(int* n_p /* out */) {

   printf("Ingrese el número de elementos del Vector A\n");
   scanf("%d", n_p);
   if (*n_p <= 0) {
      fprintf(stderr, "Order should be positive\n");
      exit(-1);
   }
}
void Allocate_vectors(
      int**  x_pp  /* out */,
      int       n     /* in  */) {
   *x_pp = (int*) malloc(n*sizeof(int));
   if (*x_pp == NULL ) {
      fprintf(stderr, "Can't allocate vectors\n");
      exit(-1);
   }
}
void Print_vector(
      int 	  b[]     /* in */,
      int     n       /* in */,
      char    title[] /* in */) {
   int i;
   printf("%s: [", title);
   for (i = 0; i < n; i++)
      printf("%d ", b[i]);
   printf("]\n");
}
void Read_vector(
      int  a[]         /* out */,
      int     n           /* out  */,
      char    vec_name[]  /* in  */) {
   int i;
   printf("Ingrese los elementos del  %s\n", vec_name);
   for (i = 0; i < n; i++)
      scanf("%i", &a[i]);
}

