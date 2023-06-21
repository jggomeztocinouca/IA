// a) Constantes necesarias para este problema, lista de operadores, tipos de datos necesarios para este problema, en particular el tipo de datos tEstado apropiado para este problema
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define V 0 // Casilla vacía
#define A 1 // Pieza A
#define B 2 // Pieza B
#define C 3 // Pieza C
#define O -1 // Obstáculo
#define A_ARRIBA 1
#define A_ABAJO 2
#define A_IZQUIERDA 3
#define A_DERECHA 4
#define B_ARRIBA 5
#define B_ABAJO 6
#define B_IZQUIERDA 7
#define B_DERECHA 8
#define C_ARRIBA 9
#define C_ABAJO 10
#define C_IZQUIERDA 11
#define C_DERECHA 12
#define NUM_PIEZAS 3
#define N 6 // Dimensión del tablero (N*N)

struct posicion{
    int fila;
    int columna;
};

typedef struct{
    int celdas[N][N];
    struct posicion centroPiezas[3];
}tEstado;

// b) Funciones para poder crear los estados inicial y final
static int puzle_inicial[N][N] =
{
    {O,V,V,C,V,V},
    {O,V,V,C,V,V},
    {V,A,V,C,V,V},
    {A,A,A,O,B,A},
    {V,A,V,B,B,B},
    {V,V,V,V,V,V},
};

static struct posicion posicionesInicial[NUM_PIEZAS] = {
    {3,1},
    {4,4},
    {0,3}
};

static int puzle_final[N][N] =
{
    {O,V,V,V,V,V},
    {O,V,V,V,V,V},
    {V,V,V,V,V,V},
    {V,A,V,O,V,C},
    {A,A,A,B,V,C},
    {V,A,B,B,B,C},
};

static struct posicion posicionesFinal[NUM_PIEZAS] = {
        {4,1},
        {5,3},
        {4,5}
};

tEstado *crearEstado(int nuevasCeldas[N][N], struct posicion nuevosCentros[NUM_PIEZAS])
{
    tEstado *estado = (tEstado*) malloc(sizeof(tEstado));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            estado->celdas[i][j] = nuevasCeldas[i][j];
        }
    }
    for(int i = 0; i < NUM_PIEZAS; i++)
    {
        estado->centroPiezas[i].fila = nuevosCentros[i].fila;
        estado->centroPiezas[i].columna = nuevosCentros[i].columna;
    }
    return estado;
}

// c) Función esVálido que determina si es o no posible la aplicación de cada operadora partir de un estado concreto solamente para 2 operadores de la pieza B.
int esValido(unsigned op, tEstado* estado){
    int valido = 0;
    switch (op) {
        default:
            break;
        case B_ABAJO:
            valido = (estado->celdas[estado->centroPiezas[B-1].fila+1][estado->centroPiezas[B-1].columna] != V && estado->centroPiezas[B-1].fila != N-1);
            break;
        case B_DERECHA:
            valido = (estado->celdas[estado->centroPiezas[B-1].fila][estado->centroPiezas[B-1].columna+2] != V && estado->centroPiezas[B-1].columna != N-2);
            break;
    }
    return valido;
}

// d) Función aplicaOperador que lleva a cabo la aplicación de cualquiera de los posibles operadores devolviendo un nuevo estado solamente para 2 operadores de la pieza B.
tEstado *aplicaOperador(unsigned op, tEstado* estado){
    tEstado *nuevoEstado = (tEstado*) malloc(sizeof (tEstado));
    memcpy(nuevoEstado, estado, sizeof(tEstado));
    int filaPieza = nuevoEstado->centroPiezas[B-1].fila; // únicamente válido para este problema, donde piden 2 operadores de B
    int columnaPieza = nuevoEstado->centroPiezas[B-1].columna;
    switch (op) {
        default:
            break;
        case B_ABAJO:
            nuevoEstado->centroPiezas[B-1].fila++;
            nuevoEstado->celdas[filaPieza-1][columnaPieza] = V;
            nuevoEstado->celdas[filaPieza][columnaPieza+1] = V;
            nuevoEstado->celdas[filaPieza][columnaPieza-1] = V;
            nuevoEstado->celdas[filaPieza+1][columnaPieza-1] = B;
            nuevoEstado->celdas[filaPieza+1][columnaPieza] = B;
            nuevoEstado->celdas[filaPieza+1][columnaPieza+1] = B;
            break;
        case B_DERECHA:
            nuevoEstado->centroPiezas[B-1].columna++;
            nuevoEstado->celdas[filaPieza-1][columnaPieza] = V;
            nuevoEstado->celdas[filaPieza-1][columnaPieza+1] = B;
            nuevoEstado->celdas[filaPieza][columnaPieza-1] = V;
            nuevoEstado->celdas[filaPieza][columnaPieza+2] = B;
            break;
    }
    return nuevoEstado;
}

// e) Función TestObjetivo para comprobar si se ha alcanzado el objetivo del problema de acuerdo al enunciado propuesto.
int TestObjetivo(tEstado* estado){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(estado->celdas[i][j] != puzle_final[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

// f) Función heurística admisible
// Opción 1: Número de piezas mal colocadas
// Opción 2: Distancia de Manhattan [Nº de filas y columnas que restan de la posición actual de una pieza a su posición final].

// e) Construye el árbol de búsqueda correspondiente a la estrategia en Anchura hasta nivel 3.
// Solución: PDF adjunto al archivo .c