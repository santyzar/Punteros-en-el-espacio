#include "jugador.h"
#include "constantes.h"

void inicializarJugador(Jugador *jugador) {
    jugador->x = ANCHO_TABLERO / 2;
    jugador->y = ALTO_TABLERO - 2;
    jugador->vidas = VIDAS_INICIALES;
}

void moverJugadorIzquierda(Jugador *jugador) {
    if (jugador->x > 1) {
        jugador->x--;
    }
}

void moverJugadorDerecha(Jugador *jugador) {
    if (jugador->x < ANCHO_TABLERO - 2) {
        jugador->x++;
    }
}
