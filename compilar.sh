#!/bin/bash
# Solicitamos un nodo con 64 cores y 256 GB de memoria durante 2 horas
# Cambiar no sbatch para menos tempo e memoria para que nos den acceso máis fácilmente. Facer unha prueba con pouco
#SBATCH -n 1 -c 64 -t 02:00:00 --mem=256G
# Ponemos nombre a nuestro trabajo para poder identificarlo.
# ATENCIÓN - Debes sustituir el NN por el número de equipo.
#SBATCH --job-name p1acgNN

# Sustituir los valores de Di y Li por los calculados para la realización de la práctica.

gcc acp1.c -o acp1 -O0 -lm

for i in {1..10}
do
	echo -e "\nEXPERIMENTO $i\n"
	for D in {1,2,16,64,512}
	do
		for L in {384,1152,10240,15360,40960,81920,163840}
		do
			./acp1 $D $L
		done
	done
done


