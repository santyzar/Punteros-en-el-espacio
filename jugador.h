#ifndef JUGADOR_H
#define JUGADOR_H

typedef struct {
    int x;
    int y;
    int vidas;
} Jugador;

void inicializarJugador(Jugador *jugador);
void moverJugadorIzquierda(Jugador *jugador);
void moverJugadorDerecha(Jugador *jugador);

#endif
