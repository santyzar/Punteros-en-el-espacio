#ifndef PANTALLA_H
#define PANTALLA_H

#include <SDL.h>
#include "juego.h"

#define TAM_CASILLA 20
#define ALTO_PANEL 60
#define ANCHO_VENTANA (ANCHO_TABLERO * TAM_CASILLA)
#define ALTO_VENTANA (ALTO_TABLERO * TAM_CASILLA + ALTO_PANEL)

typedef struct
{
    SDL_Window *ventana;
    SDL_Surface *superficie;
} Pantalla;

int iniciarPantalla(Pantalla *pantalla);

int dibujarTablero(Pantalla *pantalla, const Juego *juego, int pausado);

void cerrarPantalla(Pantalla *pantalla);

#endif
