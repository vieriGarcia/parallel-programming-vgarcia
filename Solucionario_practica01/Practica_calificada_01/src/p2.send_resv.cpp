/*
 ============================================================================
 Name        : Estudiando.c
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
double f(double x){
	return x*x;
}
int main(void) {
	int rank,size;
	int n;
	int *mess;
	MPI_Comm comm=MPI_COMM_WORLD;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(comm,&rank);
	MPI_Comm_size(comm,&size);

	if(rank==0){
		mess= new int[size-1];
		for(int u=1;u<size;u++){
			printf("Ingresa número para el proceso %d :",u);
			cin>>mess[u-1];
		}
		for(int u=1;u<size;u++){
		 MPI_Send(&mess[u-1],1, MPI_INT,u,0,comm);
		}

	}else{
		MPI_Recv(&n,1, MPI_INT,0,0,comm,NULL);
		printf("Mensaje recibido por proceso %d -> %d\n",rank,n);
	}


	MPI_Finalize();
	return 0;
}

