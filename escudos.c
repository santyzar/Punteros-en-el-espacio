#include "escudos.h"
#include "constantes.h"

void inicializarEscudos(Escudo escudos[], int cantidad)
{
    int i;
    int yBase = ALTO_TABLERO - 5;

    for (i = 0 ;  i < cantidad ; i ++)
    {
        int idEscudo = i / (ESCUDOS_BLOQUES_ANCHO * ESCUDOS_BLOQUES_ALTO);
        int indiceBloque = i % 6;

        int filalocal = indiceBloque / ESCUDOS_BLOQUES_ANCHO;
        int colLocal = indiceBloque % ESCUDOS_BLOQUES_ANCHO;

        escudos[i].x = ESCUDOS_X_INICIAL + (idEscudo * ESCUDO_SEPARACION) + colLocal;
        escudos[i].y = yBase + filalocal;
        escudos[i].salud = SALUD_ESCUDO;
    }
}
