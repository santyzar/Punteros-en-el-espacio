#ifndef COLISIONES_H
#define COLISIONES_H

#include "disparos.h"
#include "enemigos.h"
#include "jugador.h"
#include "escudos.h"

void revisarColisionesDisparosEnemigos(Disparo disparos[], int cantidadDisparos,
                                        Enemigo *enemigos, int filas, int columnas,
                                        int *puntaje);

void revisarColisionesDisparosJugador(Disparo disparos[], int cantidadDisparos, Jugador *jugador, int *jugando);

void revisarColisionesPorDisparos(Disparo disparos[], int cantidadDisparos);

void revisarColisionesDisparosEscudos(Disparo disparos[], int cantidadDisparos, Escudo escudos[], int cantidadEscudos);
#endif
