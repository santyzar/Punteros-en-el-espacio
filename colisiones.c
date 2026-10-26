#include "colisiones.h"

#define PUNTOS_POR_ENEMIGO 10

void revisarColisionesDisparosEnemigos(Disparo disparos[], int cantidadDisparos,
                                        Enemigo *enemigos, int filas, int columnas,
                                        int *puntaje) {
    Disparo *disparoActual = disparos;
    Disparo *finDisparos = disparos + cantidadDisparos;

    while (disparoActual < finDisparos) {
        if (disparoActual->activo && disparoActual->esDelJugador) {
            Enemigo *enemigoActual = enemigos;
            Enemigo *finEnemigos = enemigos + filas * columnas;

            while (enemigoActual < finEnemigos) {
                if (enemigoActual->activo &&
                    enemigoActual->x == disparoActual->x &&
                    enemigoActual->y == disparoActual->y) {
                    enemigoActual->activo = 0;
                    disparoActual->activo = 0;
                    *puntaje += PUNTOS_POR_ENEMIGO;
                }
                enemigoActual++;
            }
        }
        disparoActual++;
    }
}

void revisarColisionesDisparosJugador(Disparo disparos[], int cantidadDisparos,
                                       Jugador *jugador, int *jugando) {
    Disparo *disparoActual = disparos;
    Disparo *finDisparos = disparos + cantidadDisparos;

    while (disparoActual < finDisparos) {
        if (disparoActual->activo && disparoActual->esDelJugador == 0) {
            if (disparoActual->x == jugador->x && disparoActual->y == jugador->y) {
                disparoActual->activo = 0;
                jugador->vidas--;
                if (jugador->vidas <= 0) {
                    *jugando = 0;
                }
            }
        }
        disparoActual++;
    }
}

void revisarColisionesPorDisparos(Disparo disparos[], int cantidadDisparos) {
    Disparo *disparoJugador = disparos;
    Disparo *finDisparos = disparos + cantidadDisparos;

    while (disparoJugador < finDisparos) {
        if (disparoJugador->activo && disparoJugador->esDelJugador) {
            Disparo *disparoEnemigo = disparos;

            while (disparoEnemigo < finDisparos && disparoJugador->activo) {
                if (disparoEnemigo->activo && disparoEnemigo->esDelJugador == 0) {
                    if (disparoJugador->x == disparoEnemigo->x &&
                        (disparoJugador->y == disparoEnemigo->y ||
                         disparoJugador->y == disparoEnemigo->y - 1 ||
                         disparoJugador->y == disparoEnemigo->y + 1)) {
                        disparoJugador->activo = 0;
                        disparoEnemigo->activo = 0;
                    }
                }
                disparoEnemigo++;
            }
        }
        disparoJugador++;
    }
}

void revisarColisionesDisparosEscudos(Disparo disparos[], int cantidadDisparos,
                                       Escudo escudos[], int cantidadEscudos) {
    Disparo *disparoActual = disparos;
    Disparo *finDisparos = disparos + cantidadDisparos;

    while (disparoActual < finDisparos) {
        if (disparoActual->activo) {
            Escudo *escudoActual = escudos;
            Escudo *finEscudos = escudos + cantidadEscudos;

            while (escudoActual < finEscudos && disparoActual->activo) {
                if (escudoActual->salud > 0 &&
                    disparoActual->x == escudoActual->x &&
                    disparoActual->y == escudoActual->y) {
                    escudoActual->salud--;
                    disparoActual->activo = 0;
                }
                escudoActual++;
            }
        }
        disparoActual++;
    }
}
