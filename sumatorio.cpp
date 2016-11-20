#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctime>
#define _(STRING) gettext(STRING)

struct arg_struct {
    int arg1;
    int arg2;
};

using namespace std;

void *prime(void * arguments);
long counter = 0;

int main(int argc, char* argv[])
{

 /* Setting the i18n environment */
  setlocale (LC_ALL, "");
  bindtextdomain ("international", "/usr/share/locale/");
  textdomain ("international");

  /* Example of i18n usage */
  printf(_("Hello World\n"));

	if(argc < 3)
	{
		printf("Error en los parametros de entrada.\n");
		printf("Uso: ./sumatorio [Max num] [Threads]\n");
		exit(-1);
	}

	int NUM_PRIME = atoi(argv[1]);
	int NUM_THREADS = atoi(argv[2]);

	if( NUM_THREADS == 0 || NUM_PRIME == 0 && NUM_THREADS > NUM_PRIME )
	{
		printf("El numero de threads no puede ser mayor que el de primeros.");
		exit(-1);
	}

	printf("\nSumatorio de los %i numeros primeros.\nUsando %i hilos.\n", NUM_PRIME, NUM_THREADS);

	int NUM_WORKLOAD = NUM_PRIME / NUM_THREADS;
	printf("Cada hilo 'm' se encarga de %i digitos.\n\n", NUM_WORKLOAD);


	pthread_t threads[NUM_THREADS];

	struct arg_struct args[NUM_THREADS];
	int start_s=clock();

	for (int i = 0; i < NUM_THREADS; i++) {
		int inicio = 1 + (NUM_WORKLOAD * i);
		int fin = NUM_WORKLOAD + (NUM_WORKLOAD * i);
		
		if(i == (NUM_THREADS-1))
		{
			fin = NUM_PRIME;
		}
		args[i].arg1 = inicio;
		args[i].arg2 = fin;
		
		if(pthread_create(&threads[i], NULL ,&prime, (void *)&args[i]) != 0)
		{
			printf("Error creando thread.\n");
			exit(-1);
		} 
	}

	for (int i = 0; i < NUM_THREADS; i++) {
       		 pthread_join(threads[i], NULL);
    	}

	printf("Resultado: %ld\n", counter);

	//int stop_s=clock();
	//cout << "Tiemo (ms): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

	printf("\nFinal de la ejecución del programa.\n");
 	return 0;
}


void *prime(void * arguments)
{
	struct arg_struct *args = (struct arg_struct *)arguments;
	
	int inicio = args -> arg1;
	int fin = args -> arg2;
	long count,sum=0;
	
	for(int i = inicio; i <= fin; i++){
		sum+=i;
	}

    
	printf("Thread %ld ha calculado los primeros desde %i hasta %i, suma parcial: %ld\n", pthread_self(), inicio, fin, sum );
	counter += sum;
}

