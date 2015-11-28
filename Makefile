# Archivo de compilación del ejemplo de solución al problema del consenso en C
# Es posible modificar estos archivos para generar un proyecto en C++
# También es posible modificar estos archivos para su uso con WinApi y ejecutar
# sobre Microsoft Windows. Consulte las notas de clase anexas.
# Author: Manuel "Nachintoch" Castillo, nachintoch.mx
# Version: 1.0, november 2015

# variables de compilación
CC = gcc
JC = javac
# note las banderas -ffixed-reg para gcc
CCFLAGS = -c -I ./include/ -O2 -ffunction-sections -fdata-sections -funit-at-a-time -ffixed-eax -ffixed-ebx -ffixed-ecx -ffixed-edx
JCFLAGS = -d bin

all :
	@echo Debe indicar 'c' para compilar el ejemplo en C o 'java' para el respectivo

c : build_c
	$(CC) lib/main.o -o bin/consensus -pthread
	chmod 774 bin/consensus

java :
	$(JC) $(JCFLAGS) src/mx/nachintoch/teaching/concurrency/ConsensusExample.java

build_c :
	$(CC) $(CCFLAGS) -x c -g src/consensus_ex.c -o lib/main.o

clean :
	rm -fr bin/*

cleanall :
	rm -fr bin/* lib/*
