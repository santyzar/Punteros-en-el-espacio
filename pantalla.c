#include <stdio.h>
#include "pantalla.h"

#define CANTIDAD_ESTRELLAS 40

typedef struct
{
    Uint8 rojo;
    Uint8 verde;
    Uint8 azul;
} Color;

const Color COLOR_FONDO    = {8, 12, 28};
const Color COLOR_PANEL    = {20, 30, 50};
const Color COLOR_JUGADOR  = {70, 210, 255};
const Color COLOR_ENEMIGO  = {190, 120, 245};
const Color COLOR_OJO      = {8, 12, 28};
const Color COLOR_ESTRELLA = {80, 100, 150};
const Color COLOR_BALA_JUGADOR = {255, 230, 90};
const Color COLOR_BALA_ENEMIGO = {255, 85, 85};
const Color COLOR_LUZ_OK       = {90, 240, 130};
const Color COLOR_LUZ_PAUSA    = {255, 230, 90};
const Color COLOR_LUZ_FIN      = {255, 85, 85};

SDL_Rect rectanguloCasilla(int x, int y, int margen)
{
    SDL_Rect rectangulo;
    rectangulo.x = x * TAM_CASILLA + margen;
    rectangulo.y = ALTO_PANEL + y * TAM_CASILLA + margen;
    rectangulo.w = TAM_CASILLA - 2 * margen;
    rectangulo.h = TAM_CASILLA - 2 * margen;
    return rectangulo;
}

void pintar(SDL_Surface *superficie, const SDL_Rect *rectangulo, Color color)
{
    Uint32 valor = SDL_MapRGB(superficie->format, color.rojo, color.verde, color.azul);
    SDL_FillRect(superficie, rectangulo, valor);
}

int iniciarPantalla(Pantalla *pantalla)
{
    int ok;

    pantalla->ventana = NULL;
    pantalla->superficie = NULL;

    /* cada paso solo se hace si el anterior salio bien */
    ok = (SDL_Init(SDL_INIT_VIDEO) >= 0);

    if (ok)
    {
        pantalla->ventana = SDL_CreateWindow("Space Invaders SDL",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
        ok = (pantalla->ventana != NULL);
        if (!ok)
        {
            SDL_Quit();
        }
    }

    if (ok)
    {
        pantalla->superficie = SDL_GetWindowSurface(pantalla->ventana);
        ok = (pantalla->superficie != NULL);
        if (!ok)
        {
            SDL_DestroyWindow(pantalla->ventana);
            pantalla->ventana = NULL;
            SDL_Quit();
        }
    }
    return ok;
}
void dibujarJugador(SDL_Surface *superficie, const Jugador *jugador)
{
    SDL_Rect cuerpo = rectanguloCasilla(jugador->x, jugador->y, 2);
    SDL_Rect canon = {cuerpo.x + 6, cuerpo.y, 4, 8};

    cuerpo.y += 7;
    cuerpo.h = 9;
    pintar(superficie, &cuerpo, COLOR_JUGADOR);
    pintar(superficie, &canon, COLOR_JUGADOR);
}

void dibujarEnemigos(SDL_Surface *superficie, const Enemigo *enemigos,
                            int cantidad)
{
    const Enemigo *actual = enemigos;
    const Enemigo *fin = enemigos + cantidad;

    while (actual < fin)
    {
        if (actual->activo)
        {
            SDL_Rect cuerpo = rectanguloCasilla(actual->x, actual->y, 2);
            SDL_Rect ojo = {cuerpo.x + 3, cuerpo.y + 5, 3, 3};

            pintar(superficie, &cuerpo, COLOR_ENEMIGO);
            pintar(superficie, &ojo, COLOR_OJO);
            ojo.x += 7;
            pintar(superficie, &ojo, COLOR_OJO);
        }
        actual++;
    }
}

void dibujarEscudos(SDL_Surface *superficie, const Escudo *escudos,
                           int cantidad)
{
    const Escudo *actual = escudos;
    const Escudo *fin = escudos + cantidad;

    while (actual < fin)
    {
        if (actual->salud > 0)
        {
            SDL_Rect bloque = rectanguloCasilla(actual->x, actual->y, 1);
            Color color = {50, (Uint8)(70 + actual->salud * 50), 90};

            pintar(superficie, &bloque, color);
        }
        actual++;
    }
}

void dibujarDisparos(SDL_Surface *superficie, const Disparo *disparos,
                            int cantidad)
{
    const Disparo *actual = disparos;
    const Disparo *fin = disparos + cantidad;

    while (actual < fin)
    {
        if (actual->activo)
        {
            SDL_Rect dibujo = rectanguloCasilla(actual->x, actual->y, 2);

            dibujo.x += 6;
            dibujo.w = 4;
            if (actual->esDelJugador)
                pintar(superficie, &dibujo, COLOR_BALA_JUGADOR);
            else
                pintar(superficie, &dibujo, COLOR_BALA_ENEMIGO);
        }
        actual++;
    }
}

void dibujarEstrellas(SDL_Surface *superficie)
{
    int i;

    for (i = 0; i < CANTIDAD_ESTRELLAS; i++)
    {
        SDL_Rect estrella = {(i * 137 + 31) % ANCHO_VENTANA,
                             ALTO_PANEL + (i * 71 + 17) % (ALTO_TABLERO * TAM_CASILLA), 1, 1};

        pintar(superficie, &estrella, COLOR_ESTRELLA);
    }
}

Color colorLuzEstado(const Juego *juego, int pausado)
{
    Color color = COLOR_LUZ_OK;

    if (juego->jugando == 0)
    {
        if (!juego->gano)
            color = COLOR_LUZ_FIN;
    }
    else if (pausado)
        color = COLOR_LUZ_PAUSA;

    return color;
}

void dibujarPanel(SDL_Surface *superficie, const Juego *juego, int pausado)
{
    SDL_Rect panel = {0, 0, ANCHO_VENTANA, ALTO_PANEL};
    SDL_Rect luz = {ANCHO_VENTANA - 40, 20, 18, 14};
    int i;

    pintar(superficie, &panel, COLOR_PANEL);

    for (i = 0; i < juego->jugador.vidas; i++)
    {
        SDL_Rect vida = {20 + i * 28, 20, 18, 14};

        pintar(superficie, &vida, COLOR_JUGADOR);
    }

    pintar(superficie, &luz, colorLuzEstado(juego, pausado));
}

const char *textoEstado(const Juego *juego, int pausado)
{
    const char *estado = "JUGANDO";

    if (pausado && juego->jugando)
        estado = "PAUSA";
    else if (juego->jugando == 0)
    {
        if (juego->gano)
            estado = "GANASTE - R para reiniciar";
        else if (juego->jugador.vidas <= 0)
            estado = "SIN VIDAS - R para reiniciar";
        else
            estado = "BASE INVADIDA - R para reiniciar";
    }
    return estado;
}

void actualizarTitulo(Pantalla *pantalla, const Juego *juego, int pausado)
{
    char titulo[240];

    snprintf(titulo, sizeof titulo,
             "Nivel %d | Puntos %d | Vidas %d | %s | A/D flechas mover | Espacio disparar | P pausa | Esc salir",
             juego->nivel, juego->puntaje, juego->jugador.vidas,
             textoEstado(juego, pausado));
    SDL_SetWindowTitle(pantalla->ventana, titulo);
}

int dibujarTablero(Pantalla *pantalla, const Juego *juego, int pausado)
{
    SDL_Surface *superficie = pantalla->superficie;

    pintar(superficie, NULL, COLOR_FONDO);
    dibujarPanel(superficie, juego, pausado);
    dibujarEstrellas(superficie);
    if (juego->enemigos != NULL)
        dibujarEnemigos(superficie, juego->enemigos,
                        juego->filasEnemigos * juego->columnasEnemigos);
    dibujarEscudos(superficie, juego->escudos, CANTIDAD_ESCUDOS);
    dibujarJugador(superficie, &juego->jugador);
    dibujarDisparos(superficie, juego->disparos, MAX_DISPAROS);
    actualizarTitulo(pantalla, juego, pausado);

    return SDL_UpdateWindowSurface(pantalla->ventana) == 0;
}

void cerrarPantalla(Pantalla *pantalla)
{
    SDL_DestroyWindow(pantalla->ventana);
    pantalla->ventana = NULL;
    pantalla->superficie = NULL;
    SDL_Quit();
}
