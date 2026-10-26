#ifndef JUEGO_H
#define JUEGO_H

#include "constantes.h"
#include "jugador.h"
#include "enemigos.h"
#include "disparos.h"
#include "escudos.h"

typedef struct {
    Jugador jugador;
    Enemigo *enemigos;
    int filasEnemigos;
    int columnasEnemigos;
    int direccionEnemigos;
    Disparo disparos[MAX_DISPAROS];
    Escudo escudos[CANTIDAD_ESCUDOS];
    int puntaje;
    int jugando;
    int gano;
    int nivel;
    int velocidadEnemigos;
    int velocidadDisparoEnemigo;
} Juego;

void inicializarJuego(Juego *juego);

void dispararJuego(Juego *juego);

void reiniciarJuego(Juego *juego);

void actualizarJuego(Juego *juego, int contadorFrames);
void liberarJuego(Juego *juego);

#endif
