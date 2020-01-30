/*
 ============================================================================
 Name        : Trabajo_02.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 Suma de vectores en paralelo usando MPI_SEND & MPI_RECV
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Verificar_error(int local_ok, char fname[], char message[],
      MPI_Comm comm);
void Leer_dimension(int* n_p, int* local_n_p, int my_rank, int comm_sz,
      MPI_Comm comm);
void Asignar_memoria_vectores(double** local_x_pp, double** local_y_pp,
      double** local_z_pp, int local_n, MPI_Comm comm);
void Leer_vector(double local_a[], int local_n, int n, char vec_name[],int comm_sz,
      int my_rank, MPI_Comm comm);
void Imprimir_vector(double local_b[], int local_n, int n, char title[],int comm_sz,
      int my_rank, MPI_Comm comm);
void Suma_paralela_vector(double local_x[], double local_y[],
      double local_z[], int local_n);

int main(int argc, char *argv[]) {
	int n, local_n;
	int comm_sz, my_rank;
	double *local_x, *local_y, *local_z;
	MPI_Comm comm;
	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);
	Leer_dimension(&n, &local_n, my_rank, comm_sz, comm);
#  ifdef DEBUG
   printf("Proc %d > n = %d, local_n = %d\n", my_rank, n, local_n);
#  endif
   Asignar_memoria_vectores(&local_x, &local_y, &local_z, local_n, comm);
   Leer_vector(local_x, local_n, n, "x",comm_sz,my_rank, comm);
   Imprimir_vector(local_x, local_n, n, "Vector x:",my_rank,comm_sz,comm);
   Leer_vector(local_y, local_n, n, "y",comm_sz,my_rank, comm);
   Imprimir_vector(local_y, local_n, n, "Vector y:",my_rank,comm_sz,comm);
   Suma_paralela_vector(local_x, local_y,local_z,local_n);
   Imprimir_vector(local_z, local_n, n, "Vector Suma:",my_rank,comm_sz,comm);
   MPI_Finalize();
   return 0;
}


void Leer_dimension(
      int*      n_p        /* out */,
      int*      local_n_p  /* out */,
      int       my_rank    /* in  */,
      int       comm_sz    /* in  */,
      MPI_Comm  comm       /* in  */) {
   int local_ok = 1;
   char *fname = "Leer_dimension";

   if (my_rank == 0) {
      printf("Cual es la dimensión del vector?\n");
      scanf("%d", n_p);
   }
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   if (*n_p <= 0 || *n_p % comm_sz != 0) local_ok = 0;
   Verificar_error(local_ok, fname,
         "n debe ser > 0 y divisible entre la cantidad de procesos", comm);
   *local_n_p = *n_p/comm_sz;
}
void Verificar_error(
      int       local_ok   /* in */,
      char      fname[]    /* in */,
      char      message[]  /* in */,
      MPI_Comm  comm       /* in */) {
   int ok;
   MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
   if (ok == 0) {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) {
         fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname,
               message);
         fflush(stderr);
      }
      MPI_Finalize();
      exit(-1);
   }
}
void Asignar_memoria_vectores(
      double**   local_x_pp  /* out */,
      double**   local_y_pp  /* out */,
      double**   local_z_pp  /* out */,
      int        local_n     /* in  */,
      MPI_Comm   comm        /* in  */) {
   int local_ok = 1;
   char* fname = "Asignar_memoria_vectores";

   *local_x_pp = malloc(local_n*sizeof(double));
   *local_y_pp = malloc(local_n*sizeof(double));
   *local_z_pp = malloc(local_n*sizeof(double));

   if (*local_x_pp == NULL || *local_y_pp == NULL ||
       *local_z_pp == NULL) local_ok = 0;
   Verificar_error(local_ok, fname, "Can't allocate local vector(s)",
         comm);
}
void Leer_vector(
      double    local_a[]   /* out */,
      int       local_n     /* in  */,
      int       n           /* in  */,
      char      vec_name[]  /* in  */,
	  int       comm_sz     /* in  */,
      int       my_rank     /* in  */,
      MPI_Comm  comm        /* in  */) {

   double* a = NULL;
   int i;
   int local_ok = 1;
   char* fname = "Leer_vector";
   a = malloc(n*sizeof(double));
   if (my_rank == 0) {
      if (a == NULL) local_ok = 0;
      Verificar_error(local_ok, fname, "No se pudo asignar memoria al vector temporal",
            comm);
      printf("Ingrese el vector %s\n", vec_name);
      for (i = 0; i < n; i++){
         scanf("%lf", &a[i]);
		 if(i<local_n){
			 local_a[i]=a[i];
		 }
      }
      for(int u=1;u<comm_sz;u++){
    	  MPI_Send(a,n, MPI_DOUBLE,u,0, comm);
      }
      free(a);
   } else {
      Verificar_error(local_ok, fname, "No se pudo asignar memoria al vector temporal",
            comm);
      MPI_Recv(a,n, MPI_DOUBLE,0,0,comm, NULL);
      for(int u=0; u<local_n;u++){
          local_a[u]=a[u+2*my_rank];
        }
   }

}
void Imprimir_vector(
      double    local_b[]  /* in */,
      int       local_n    /* in */,
      int       n          /* in */,
      char      title[]    /* in */,
      int       my_rank    /* in */,
	  int       comm_sz    /* in */,
      MPI_Comm  comm       /* in */) {

   double* b = NULL;
   double* b_temp = NULL;
   int i;
   int local_ok = 1;
   char* fname = "Imprimir_vector";

   if (my_rank != 0) {
	   Verificar_error(local_ok, fname, "No se pudo asignar memoria al vector temporal",
	               comm);
	         MPI_Send(local_b,local_n, MPI_DOUBLE,0,0, comm);
   } else {

      b = malloc(n*sizeof(double));
      b_temp= malloc(local_n*sizeof(double));
           if (b == NULL) local_ok = 0;
           Verificar_error(local_ok, fname, "No se pudo asignar memoria al vector temporal",
                 comm);
           for(int r=0;r<local_n;r++){
               b[r]=local_b[r];
           }
           for(int u=1;u<comm_sz;u++){
         	    MPI_Recv(b_temp,local_n, MPI_DOUBLE,u,0,comm, NULL);
         	    for(int r=2*u;r<local_n+2*u;r++){
         	        b[r]=b_temp[r-2*u];
         	   }
           }
           printf("%s", title);
           for (i = 0; i < n; i++)
              printf("%f.2 ", b[i]);
           printf("\n");
           free(b);
   }
}
void Suma_paralela_vector(
      double  local_x[]  /* in  */,
      double  local_y[]  /* in  */,
      double  local_z[]  /* out */,
      int     local_n    /* in  */) {
   int local_i;

   for (local_i = 0; local_i < local_n; local_i++)
      local_z[local_i] = local_x[local_i] + local_y[local_i];
}
