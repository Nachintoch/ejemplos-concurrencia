Ejemplos de uso de herramientas provistas en Java y C (aplicables en C++
también) para resolver problemas de cómputo en paralelo, particularmente cómputo
concurrente.

Para compilar los ejemplos, se ha provisto de un Makefile. Aunque el compilado
y las instrucciones en el archivo 'consensus_ex.c' están orientadas a sistemas
basados en UNIX (desarrollados y probados usando GNU/Debian 8), puede compilarse
sin mayor problema el .java en un sistema Windows, y cambiando los encabezados
y algunas funciones, también es posible usar el ejemplo en C con la WinApi,
y así producir un .exe ejecutable en Windows. Además, el código en C puede
usarse para generar un pŕoyecto en C++ también.

Consulte las notas para mayor información:
http://nachintoch.mx/teaching/texts/23_nov%20-%20Ejemplos%20en%20Java%20y%20C%20soluci%C3%B3n%20al%20problema%20del%20consenso.pdf

Estos ejemplos, pretenden ayudarle a entender mejor cómo se usan las
herramientas de paralelización y concurrencia que nos ofrecen estos lenguajes.

Autor: Manuel "Nachintoch" Castillo
Contacto: nachintoch.mx		contact@nachintoch.mx

------------------------ COMPILACIÓN -------------------------------------------
1) Java
Para compilar el proyecto en Java, necesitamos por supuesto tener Java instalado
en nuestro equipo. Si no tienes Java instalado, puedes conseguirlo aquí:
http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html

Con java instalado, simplemente hacemos lo siguiente:
- Usando una consola, nos movemos hasta el directorio raíz del proyecto.
- Damos el comando: make java

Si eres usuario Windows, es posible que cuentes con un interprete para el
makefile, pero puedes hacer la compilación manualmente usando en su lugar el
siguiente comando explícitamente:
- javac -d bin src/mx/nachintoch/teaching/concurrency/ConsensusExample.java

2) C
Para compilar el proyecto en C, necesitamos tener instalado GCC (SO basados en
UNIX solamente. Usuarios Windows deberán modificar el código fuente para que
funcione apropiadamente) y un intérprete para el Makefile (cómo el comando
make).

Cumpliendo estos requisitos, hacemos lo siguiente:
- Usando una consola, nos movemos hasta el directorio raíz del proyecto.
- Damos el comando: make c

Si eres usuario de un sistema POSIX distinto de Linux (por ejemplo Mac OS X),
es posible que al compilar obtengas errores del tipo "undefined reference". Si
esto ocurre, cambia la bandera -lpthread en la línea 19 del Makefile por -lrt
Es posible que tengas que cambiar el nombre y/o número de parámetros también
para hacerlo funcionar adecuadamente.

---------------------------- USO -----------------------------------------------
1) Java
Habiendo compilado el proyecto en Java, usamos una consola y hacemos lo
siguiente:
- Nos movemos hasta el directorio raíz del proyecto.
- Damos el comando: java -cp bin mx.nachintoch.teaching.concurrency.ConsensusExample <pase como parámetro el número de hilos que desea hacer participar en el consenso>

Lo anterior es válido en Windows también.

2) C
Habiendo compilado el proyecto en C, usamos una consola y hacemos lo siguiente:
- Nos movemos hasta el directorio raíz del proyecto.
- Damos el comando: bin/consensus <pase como parámetro el número de procesos que desea hacer participar en el consenso>

------------ OTRAS HERRAMIENTAS INCLUIDAS EN EL MAKEFILE -----------------------
- Para borrar los archivos compilados use
make clean

- Para borrar todos los archivos generados, use
make cleanall 
