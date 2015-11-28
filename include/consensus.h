#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
 * Además de lo ya estudiado, stdlib contiene atoi, el equivalente en C a
 * parseInt. También contiene contiene srand y rand, equivalentes a Random en C.
 */
#include <stdlib.h>
// Usamos el tiempo actual del sistema como semilla para generar los aleatorios
#include <time.h>
#include <semaphore.h>
// Nos permite acceder directamente al estado de error de la aplicación
#include <errno.h>
// esta es la biblioteca que usamos para memoria (variables) compartidas
#include <sys/mman.h>
// esta biblioteca contiene a O_CREAT
#include <fcntl.h>
// bibliteca para detectar señales como ctrl +C
#include <signal.h>
/*
 * el siguiente es el nombre simbólico del semáforo que comparten los procesos.
 * Los semáforos POSIX nombrados son almacenados por el sistema, y en cierta
 * forma son de acceso público, por loq ue es buena idea que el semáforo tenga
 * un nombre suficientemente largo cómo para creer que será único, o podría
 * interferir con el semáforo de otra aplicación.
 */
#define MUTEX "/nachintoch_posix_mutex_example"

void call(int);
void stop_handler(int);
