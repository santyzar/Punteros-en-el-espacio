#ifndef ENEMIGOS_H
#define ENEMIGOS_H

typedef struct {
    int x;
    int y;
    int activo;
} Enemigo;

Enemigo *crearEnemigos(int filas, int columnas);

void liberarEnemigos(Enemigo *enemigos);

void moverEnemigos(Enemigo *enemigos, int filas, int columnas, int *direccion);

int quedanEnemigosActivos(const Enemigo *enemigos, int filas, int columnas);
int contarEnemigosActivos(const Enemigo *enemigos, int filas, int columnas);
int enemigosLlegaronAlFondo(const Enemigo *enemigos, int filas, int columnas);

int elegirColumnaConEnemigos(const Enemigo *enemigos, int filas, int columnas);

Enemigo *enemigoMasBajoEnColumna(Enemigo *enemigos, int filas, int columnas, int columna);

#endif
