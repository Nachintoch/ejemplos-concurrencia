#include "consensus.h"

/*
 * Esta es una implementación equivalente a la dada en Java con estasa notas.
 * El propósito de este programa, es ilustrar el uso de las herramientas de las
 * que disponemos en C y C++ para cómputo concurrente. En particular, vamos a
 * implementar la solución al problema del consenso presentada en el libro
 * Synchronization algorithms and concurrent programming, del autor Gadi
 * Taubenfeld, de editorial Pearson Prentice Hall, en las páginas 310 y 311.
 *
 * Author Manuel "Nachintoch" Castillo, nachintoch.mx
 * Version 1.0, november 2015
 */

/*
 * en el ejemplo en Java, estas son las variables estáticas de clase. Note que
 * para facilidad de comparación y entendimiento, se declaran en el mismo orden
 * que en la clase de Java.
 */

unsigned int *decision;
sem_t *mutex;
unsigned int *x;
unsigned int *y;
unsigned int *finish;
unsigned int i = 0;
unsigned short int aux;

int main(int argc, char *args[])
{
    if(argc <= 1)
    {
        /*
		 * Comprobamos que al menos se pasó un parámetro de consola.
		 * De ser así, intentamos leer el primero como el número de procesos
		 * participantes en el problema del consenso. Si no, chilla
         */
        fprintf(stderr, "At least, there must be one numeric parameter to be used as contenders number\n");
        exit(EXIT_FAILURE);
    }//comprueba el número de argumentos
    unsigned int procnum = atoi(args[1]);
    if(procnum <= 1)
    {
    	fprintf(stderr, "At least, two processes must be created. Use as parameter any number grather than 1");
    	exit(EXIT_FAILURE);
    }//si se pidio crear menos de 2 hijos, chilla
    /*
     * vea la descripción de stop_handler para más información de la siguente
     * linea. SIGINT es la señal que le envía el sistema a la aplicación cuando
     * el usuario hace explícito que quiere interrumpirla (por ejemplo, dando
     * ctrl +c)
     */
    signal(SIGINT, stop_handler);
    // hay que declarar las variables compartidas como tales
    decision = mmap(NULL, sizeof(*decision), PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	// notemos que tenemos apuntadores a las variables compartidas
    *decision = 0;
    mutex = sem_open(MUTEX, O_CREAT, 0644, 1);
    x = mmap(NULL, sizeof(*x), PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *x = 0;
    y = mmap(NULL, sizeof(*y), PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *y = 0;
    finish = mmap(NULL, sizeof(*finish), PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *finish = 0;
    unsigned int failures = 0;
    pid_t cpid;
    for(; i < procnum; i++)
    {
        if((cpid = fork()) < 0)
        {
            failures++;
            fprintf(stderr, "Houston, we had a problem when creating %u of %u child processes\n", failures, procnum);
        }
		else if(cpid == 0)
		{
            i -= failures; // aseguramos conteo continuo
            i++;// note que estas 2 instrcciones no altera el i del padre
            srand(time(NULL)); // new Random
            mutex = sem_open(MUTEX, 0); // recuperamos el semáforo
            aux = rand();
            call(aux %2); // call contiene un exit, este termina el proceso
        }//actúa de acuerdo en dónde siga: padre, hijo o error santo	
    }//pone a competir a tantos procesos hijos como haya sido indicado
    /*
	 * La combinación de todos los exit en los call de los hijos y los
	 * waitpid, son nuestro Future en C. Vamos a esperar en un ciclo, que no es
	 * una espera ocupada, recordemos que waitpid se encarga de bloquear
	 * eficientemente el proceso padre. El ciclo nos sirve para comprobar que
	 * no hay ningún proceso hijo restante
	 */
	while(cpid = waitpid(-1, NULL, 0))
    {
        if(errno == ECHILD)
        {
            break;
        }// si en efecto no hay más hijos o si un hijo murio anormalmente
    }//esperamos a un evento que reanude al padre
    fprintf(stdout, "Process decided %u\n", *decision);
    //liberamos la memoria usada por las variables compartidas
    munmap(decision, sizeof(*decision));
    sem_unlink(MUTEX);
    munmap(x, sizeof(*x));
    munmap(y, sizeof(*y));
    munmap(finish, sizeof(*finish));
    return 0;
}//main

void call(int input)
{
	register int *ini;
    register int *decisioni;
    /*
     * ini y decisioni son apuntadores no inicializados, hay que inicializarlos.
     * Si no, al intentar asignar un valor en su dirección de memoria, lo
     * haremos en quien sabe que dirección de memoria y seguramente es inválida.
     */
    ini = malloc(sizeof(*ini));
    decisioni = malloc(sizeof(*decisioni));
    *ini = input;
    *y = *x = 0;
    if(i == 1 || i == 2)
    {
        sem_wait(mutex); //down o acquire
        if(*x)
        {
            *decisioni = 1;
            sem_post(mutex); // up o release
        }
        else
        {
            *x = *ini;
            sem_post(mutex);
            sem_wait(mutex);
            if(!(*y))
            {
                *y = 1;
                *decisioni = *ini;
                sem_post(mutex);
            }
            else
            {
                sem_post(mutex);
                if(!(*x))
                {
                    *decisioni = 0;
                }//si x es 0
            }//si y es 1 o 0
        }//si x es 0 o 1
        *decision = *decisioni;
        *finish = 1;
    }
    else
    {
        while(!(*finish))
        {
        	sleep(100);
        }//espera 100 milisegundos, a ver si ya acabaron los distingidos
    }//si se trata de alguno de los procesos distinguidos que deciden
    exit(EXIT_SUCCESS); // el hijo se suicida indicando que sí pudo
}//call

/*
 * La siguiente función no tiene un propósito estricto relacionado al problema
 * del consenso resuelto arriba. Sin embargo, cuando creamos un semáforo POSIX
 * nombrado, algunos SO (cómo GNU/Debian por ejemplo) mantienen "vivos" los
 * semáforos nombrados creados en el directorio "dev/shm", por lo que si
 * interrumpimos una aplicación que mantiene un semáforo nombrado, es posible
 * que el semáforo permanezca vivo en el sistema y al reinstanciar la aplicación
 * habrá anomalias en su comportamiento que no se resolverán hasta que
 * reiniciemos el sistema, o hasta que alguna apiación llame a sem_unlink(name)
 * con el mismo nombre del semáforo creado.
 *
 * Esta función precisamente se encarga de dicha acción: detecta señales de
 * interrupción y elimina el semáforo adecuadamente.
 */
void stop_handler(int signal)
{
	sem_unlink(MUTEX);
	munmap(decision, sizeof(*decision));	
    munmap(x, sizeof(*x));
    munmap(y, sizeof(*y));
    munmap(finish, sizeof(*finish));
	/*
	 * alteramos el comportamiento de respuesta de la interrupción, pero el
	 * usuario espera terminar el programa. Por eso la siguiente instrucción.
	 */
	fprintf(stdout, "Abortado\n");
	exit(EXIT_SUCCESS);
}//stop_handler
