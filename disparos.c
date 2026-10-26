#include "disparos.h"
#include "constantes.h"

void inicializarDisparos(Disparo disparos[], int cantidad) {
    int i;
    for (i = 0; i < cantidad; i++) {
        disparos[i].activo = 0;
    }
}

int dispararJugador(Disparo disparos[], int cantidad, int xJugador, int yJugador) {
    int i;
    int disparoCreado = 0;
    int activosDelJugador = 0;

    for (i = 0 ; i < cantidad ; i++)
    {
        if(disparos[i].activo && disparos[i].esDelJugador)
            activosDelJugador++;
    }

    if (activosDelJugador < MAX_DISPAROS_JUGADOR) {
        for (i = 0; i < cantidad && disparoCreado == 0; i++) {
            if (disparos[i].activo == 0) {
                disparos[i].x = xJugador;
                disparos[i].y = yJugador - 1;
                disparos[i].activo = 1;
                disparos[i].esDelJugador = 1;
                disparoCreado = 1;
            }
        }
    }
    return disparoCreado;
}

int dispararEnemigo(Disparo disparos[], int cantidad, int xEnemigo, int yEnemigo)
{
    int i;
    int disparoCreado = 0;

    for (i = 0 ; i < cantidad && disparoCreado == 0; i++)
        {
            if(disparos[i].activo == 0)
            {
                disparos[i].x = xEnemigo;
                disparos[i].y = yEnemigo + 1;
                disparos[i].activo = 1;
                disparos[i].esDelJugador = 0;
                disparoCreado = 1;
            }
        }
    return disparoCreado;
}

void moverDisparos(Disparo disparos[], int cantidad) {
    int i;
    for (i = 0; i < cantidad; i++) {
        if (disparos[i].activo) {
            if (disparos[i].esDelJugador) {
                disparos[i].y--;
            } else {
                disparos[i].y++;
            }
            if (disparos[i].y <= 0 || disparos[i].y >= ALTO_TABLERO - 1) {
                disparos[i].activo = 0;
            }
        }
    }
}

