////////////////////////////////////
//    EXPERIMENTO CON INTEGERS    //
////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdalign.h>
#include "counter.h"

#define CLS 64 //Tamaño dunha línea de caché
#define E 16 //Número de integers que caben en 1 línea caché

//Función de apoyo a qsort
int compararValores(const void *a, const void *b)   
{
    double arg1 = *(const double*)a;
    double arg2 = *(const double*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

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
    int R = (D * sizeof(int) < CLS) ?  L * (E/D) : L; //Numero de elementos aos que accedemos
    int tamA = (R - 1) * D + 1; //Tamaño total de A
    int* A = (int*) aligned_alloc(CLS, tamA * sizeof(int)); //Vector sobre o que realizaremos a operación
    alignas(CLS) double S[10]; //Vector cos resultados obtidos
    alignas(CLS) double T[10]; //Vector cos tempos de cada cálculo
    double s_med = 0, t_med = 0; //Variables cos valores medios
    int* ind = (int*) aligned_alloc(CLS, R * sizeof(int)); //Vector de índices


    //Comprobamos erros na reserva de memoria
    if(A == NULL || ind == NULL)
    {
        printf("Error reservando memoria, cerrando el programa...\n");
        exit(EXIT_FAILURE);
    }

    //Inicializamos ind
    for(int i = 0; i < R; i++)
    {
        ind[i] = i * D;
    }    

    //Inicializamos S
    for(int i = 0; i < 10; i++)
    {
        S[i] = 0;
    }

    //Inicializamos A
    for(int i = 0; i < tamA; i++)
    {
        A[i] = (rand() % 201) - 100; //Número aleatorio en (-100, 100), sólo enteiros
    }

    //Facemos a reducción e medimos tempo
    for(int i = 0; i < 10; i++)
    {
        start_counter();

        for(int j = 0; j < R; j++)
        {
            S[i] += A[ind[j]]; //Sumamos o valor de A correspondente
        }

        T[i] = get_counter();
    }

    //Calculamos valores medios
    for(int i = 0; i < 10; i++)
    {
        s_med += S[i];
    }

    //Ordenamos vector de tiempos para obtener los 3 mejores y hacer la media con ellos
    qsort(T, 10, sizeof(double), compararValores); 

    s_med /= 10;
    t_med = pow(T[0] * T[1] * T[2], 1.0/3); //Calculamos tiempo medio como resultado de la media geométrica de los 3 mejores valores

    printf("(D: %d, L: %d) Resultado medio: %lf. Tiempo medio: %1.10lf\n", D, L, s_med, t_med);

    //Liberamos vectores dinámicos
    free(A);
    free(ind);
    
    return 0;
}