//////////////////////////////////////////
//    EXPERIMENTO CON ACCESO DIRECTO    //
//////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdalign.h>
#include "counter.h"

#define CLS 64 //Tamaño dunha línea de caché
#define E 8 //Número de doubles que caben en 1 línea caché

int main(int argc, char** argv)
{

    //Comprobamos que os argumentos sean suficientes
    if(argc < 3)
    {
        perror("¡¡¡INSUFICIENTES ARGUMENTOS DE ENTRADA!!! ABORTANDO...");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int D = atoi(argv[1]); //Valor de salto, usado para estudiar a localidad
    int L = atoi(argv[2]); //Valor para o que se toman as medidas de ciclos
    int R = (D * sizeof(double) < CLS) ?  L * (E/D) : L; //Numero de elementos aos que accedemos
    int tamA = (R - 1) * D + 1; //Tamaño total de A
    double *A = (double*) aligned_alloc(CLS, tamA * sizeof(double)); //Vector sobre o que realizaremos a operación
    alignas(CLS) double S[10]; //Vector cos resultados obtidos
    alignas(CLS) double T[10]; //Vector cos tempos de cada cálculo
    double s_med = 0, t_med = 0; //Variables cos valores medios

    //Comprobamos erros na reserva de memoria
    if(A == NULL)
    {
        printf("Error reservando memoria, cerrando el programa...\n");
        exit(EXIT_FAILURE);
    }


    //Inicializamos S
    for(int i = 0; i < 10; i++)
    {
        S[i] = 0;
    }

    //Inicializamos A
    for(int i = 0; i < tamA; i++)
    {
        A[i] = 1.0 + (double) rand() / ((double) RAND_MAX + 1.0); //Número aleatorio en [1,2] (hay que cambialo para que sea en [1,2))

        //Como se usan tamén negativos pero a fórmula anterior só da positivos, collemos un valor aleatorio e cambiamos signo si da impar
        if(rand() % 2 != 0)
        {
            A[i] = -A[i];
        }
    }

    //Facemos a reducción e medimos tempo
    for(int i = 0; i < 10; i++)
    {
        start_counter();

        for(int j = 0; j < R; j++)
        {
            S[i] += A[D * j]; //Sumamos o valor de A correspondente
        }

        T[i] = get_counter();
    }

    //Calculamos valores medios
    for(int i = 0; i < 10; i++)
    {
        s_med += S[i];
        t_med += T[i];
    }

    s_med /= 10;
    t_med /= 10;

    printf("(D: %d, L: %d) Resultado medio: %lf. Tiempo medio: %1.10lf\n", D, L, s_med, t_med);

    //Liberamos vectores dinámicos
    free(A);
    
    return 0;
}