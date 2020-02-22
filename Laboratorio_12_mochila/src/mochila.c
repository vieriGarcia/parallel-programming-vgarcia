/*
 ============================================================================
 Name        : Laboratorio_12.c
 Author      : Vieri Garcia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 Compilar :gcc -g -Wall -o Laboratorio_12 mochila.c -lpthread
 Ejecutar :./Laboratorio_12
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
// P = capacidad de la mochila , n = número de elementos, max= mayor valor obtenido, best_peso= peso ocupado por la solución
int P=0,n,max=0,best_peso=0;
// ov = arreglo que almacena los valores de cada elemento
int ov[100];
// op= arreglo que almacena los pesos de cada elemento
int op[100];
// archivo_existe: indica si el nombre de archivo ingresado es correcto o no
int archivo_existe=1;
void decimal_to_binario(int dec, int binario[],int n){
	int res=dec;
	for(int i=n-1;i>=0;i--){
		binario[i]=res%2;
		res=res/2;
	}
}
long potencia(int base, int exponente){
	long potencia=1;
   #pragma omp parallel for
	for(int i=0;i<exponente;i++){
		potencia=potencia*base;
	}
	return potencia;
}
void representar_problema(){
printf("Representación del problema \n\nCapacidad:%d kg\n",P);
printf("| N° | %-5s | %-5s |\n","Peso(kg)","Valor");
#pragma omp parallel for
  for (int i = 0; i < n; i++)
  {
	printf("| %d° | %5d    | %3d   |\n",i+1,op[i],ov[i]);
  }
}
void leer_archivo(int *n, char filename[50]){
	FILE * fp;
	char buffer[100];
	int cont=0;
	fp=fopen(filename,"r+");
	if (fp == NULL){
	  archivo_existe=0;
	  printf("\nError de apertura del archivo. \n\n");
	}else{
		while (feof(fp) == 0)
		{
		  fgets(buffer,100,fp);
		  if(cont==0){
			P=(int)buffer[2]-48;
		  }else{

			  ov[cont-1]=(int)buffer[2]-48;
			  op[cont-1]=(int)buffer[4]-48;
		  }
		  cont++;
		}
	}
	*n=cont-1;
	fclose(fp);
}

int main (int argc, char *argv[]) {

  //int numThreads, tid;
  long num_alt=0;
  int valor,peso;
  int *sol=NULL;
  int *alt_sol=NULL;
  char file_name[50]= "";
  printf("Ingrese nombre del archivo:");
  scanf("%s",&file_name);
  //Leer datos del archivo
  leer_archivo(&n,file_name);
  if(archivo_existe==1){
	  alt_sol=malloc(n*sizeof(int));
	    sol=malloc(n*sizeof(int));
	    num_alt=potencia(2,n);
	    #pragma omp parallel for  private(valor,peso,alt_sol)
	    for(int i=0;i<num_alt;i++){
	  	  decimal_to_binario(i,alt_sol,n);
	  	  valor=0;
	  	  peso=0;
	  	  for(int j=0;j<n;j++){
	  		  peso+=op[j]*alt_sol[j];
	  		  valor+=ov[j]*alt_sol[j];
	  	  }
	  	  if(peso>P)valor=0;
	  	  if(max<valor){
	  		  max=valor;
	  		  best_peso=peso;
	  		  for(int i=0;i<n;i++){
	  			  sol[i]=alt_sol[i];
	  		   }
	  	  }
	    }
	    representar_problema();
	    printf("\nNúmero de alternativas solución:%ld\n",num_alt);
	    printf("\nSolución:\nElegir los elementos de orden :");
	    for(int i=0;i<n;i++){
	    	  if(sol[i]!=0){
	    		  if(i==n-3){
	    			  printf("%d°y ",i+1);
	    		  }else{
	    			printf("%d°,",i+1);
	    		  }
	    	  }
	    }
	    printf ("\b \b");
	    printf("\n\nDetalles de la solución:\nEl peso ocupado por la solución es %d kg\n",best_peso);
	    printf("El valor de la solución es %d\n",max);
  }


 return 0;
}




