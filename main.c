#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pantalla.h"   /* trae SDL y las estructuras del juego */

int main(int argc, char *args[])
{
    Juego juego;
    Pantalla pantalla;
    SDL_Event evento;
    int abierto = 1;            /* pasa a 0 al cerrar la ventana o apretar Esc / Q */
    int pausado = 0;
    int contadorFrames = 0;
    int esperaDisparo = 0;      /* vueltas que faltan para poder disparar de nuevo */
    int disparoPendiente = 0;   /* se apreto espacio y todavia no se atendio */
    int error = 0;
    const Uint8 *teclas;
    Uint32 inicioVuelta, tiempoUsado;
    (void)argc;                 /* no los usamos, pero SDL2main pide esta firma */
    (void)args;

    srand((unsigned int)time(NULL));
    if (iniciarPantalla(&pantalla) == 0)
    {
        printf("Error al iniciar la pantalla: %s\n", SDL_GetError());
        return 1;
    }
    inicializarJuego(&juego);

    /* cada vuelta: eventos -> teclado -> reglas -> dibujo -> espera */
    while (abierto != 0 && error == 0)
    {
        inicioVuelta = SDL_GetTicks();   /* hora en que arranca la vuelta, en ms */

        /* eventos: cosas que pasan una sola vez (cerrar, perder foco, apretar una tecla) */
        while (SDL_PollEvent(&evento) != 0)
        {
            if (evento.type == SDL_QUIT)
                abierto = 0;
            else if (evento.type == SDL_WINDOWEVENT &&
                     evento.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                pausado = 1;                  /* si cambia de ventana, pausa */
            else if (evento.type == SDL_KEYDOWN && evento.key.repeat == 0)
            {
                /* !repeat: ignora el auto-repetir de mantener la tecla apretada */
                if (evento.key.keysym.sym == SDLK_ESCAPE || evento.key.keysym.sym == SDLK_q)
                    abierto = 0;
                else if (evento.key.keysym.sym == SDLK_p && juego.jugando != 0)
                    pausado = !pausado;
                else if (evento.key.keysym.sym == SDLK_SPACE && juego.jugando != 0 && pausado == 0)
                    disparoPendiente = 1;     /* anotamos el toque aunque sea cortito */
                else if (evento.key.keysym.sym == SDLK_r && juego.jugando == 0)
                {
                    reiniciarJuego(&juego);   /* libera y vuelve a crear la partida */
                    contadorFrames = 0;
                    esperaDisparo = 0;
                    disparoPendiente = 0;
                    pausado = 0;
                }
            }
        }

        if (abierto != 0 && juego.enemigos == NULL)
        {
            printf("No se pudo reservar memoria para los enemigos.\n");
            error = 1;
        }

        if (abierto==1 && error ==0)
        {
            /* teclado: como estan las teclas ahora (sirve para mantener apretado) */
            teclas = SDL_GetKeyboardState(NULL);
            if (juego.jugando != 0 && pausado == 0)
            {
                int izquierda = teclas[SDL_SCANCODE_A] || teclas[SDL_SCANCODE_LEFT];
                int derecha = teclas[SDL_SCANCODE_D] || teclas[SDL_SCANCODE_RIGHT];

                if (izquierda==1 && derecha==0)
                    moverJugadorIzquierda(&juego.jugador);
                else if (derecha==1 && izquierda==0)
                    moverJugadorDerecha(&juego.jugador);

                if (esperaDisparo > 0)
                    esperaDisparo--;
                if ((teclas[SDL_SCANCODE_SPACE] != 0 || disparoPendiente != 0) && esperaDisparo == 0)
                {
                    dispararJuego(&juego);
                    esperaDisparo = 4;        /* 4 vueltas de 50 ms = 200 ms entre disparos */
                }
                actualizarJuego(&juego, contadorFrames);
                contadorFrames++;
            }
            disparoPendiente = 0;

            /* se dibuja siempre: en pausa o al terminar se ve el ultimo estado */
            if (dibujarTablero(&pantalla, &juego, pausado) == 0)
            {
                printf("Error al dibujar: %s\n", SDL_GetError());
                error = 1;
            }

            /* esperar lo que falte para completar los 50 ms de la vuelta */
            tiempoUsado = SDL_GetTicks() - inicioVuelta;
            if (tiempoUsado < DELAY_LOOP_MS)
                SDL_Delay(DELAY_LOOP_MS - tiempoUsado);
        }
    }

    liberarJuego(&juego);
    cerrarPantalla(&pantalla);
    return error;
}
