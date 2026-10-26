#ifndef ESCUDOS_H_INCLUDED
#define ESCUDOS_H_INCLUDED

typedef struct
{
    int x;
    int y;
    int salud;
} Escudo;

void inicializarEscudos(Escudo escudos[], int cantidad);

#endif
