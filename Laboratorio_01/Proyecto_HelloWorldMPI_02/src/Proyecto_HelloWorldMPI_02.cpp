/*
 ============================================================================
 Name        : Proyecto_01_HelloWorldMPI.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
  Compile Command:
 	$ mpiCC -g -Wall -o Proyecto_01_HelloWorldMPI Proyecto_01_HelloWorldMPI.cpp
	$ mpiexec  ./Proyecto_01_HelloWorldMPI
	$ mpiexec -n 10 ./Proyecto_01_HelloWorldMPI
 ============================================================================
 */
#include "mpi.h"
#include <iostream>
using namespace std;
 
int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv); // Inicializacion del entorno MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero de procesos en el comunicador global
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos la identificacion de nuestro proceso en el comunicador global

    cout<<"¡Hola Mundo desde el proceso "<<rank<<" de "<<size<<" que somos!"<<endl;

    // Terminamos la ejecucion de los procesos, despues de esto solo existira
    // la hebra 0
    // ¡Ojo! Esto no significa que los demas procesos no ejecuten el resto
    // de codigo despues de "Finalize", es conveniente asegurarnos con una
    // condicion si vamos a ejecutar mas codigo (Por ejemplo, con "if(rank==0)".
    MPI_Finalize();

    return 0;
}
