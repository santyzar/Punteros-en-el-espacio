#ifndef DISPAROS_H
#define DISPAROS_H

typedef struct {
    int x;
    int y;
    int activo;
    int esDelJugador;
} Disparo;

void inicializarDisparos(Disparo disparos[], int cantidad);
int dispararJugador(Disparo disparos[], int cantidad, int xJugador, int yJugador);
int dispararEnemigo(Disparo disparos[], int cantidad, int xEnemigo, int yEnemigo);
void moverDisparos(Disparo disparos[], int cantidad);

#endif
