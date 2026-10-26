#include <stdlib.h>
#include "enemigos.h"
#include "constantes.h"

#define ENEMIGO_X_INICIAL         2
#define ENEMIGO_Y_INICIAL         1
#define SEPARACION_HORIZONTAL     3
#define FILAS_LIBRES_SOBRE_JUGADOR 3

Enemigo *crearEnemigos(int filas, int columnas) {
    Enemigo *enemigos = malloc(filas * columnas * sizeof(Enemigo));
    Enemigo *actual = NULL;
    int f, c;

    if (enemigos != NULL) {
        for (f = 0; f < filas; f++) {
            for (c = 0; c < columnas; c++) {
                actual = enemigos + f * columnas + c;
                actual->x = ENEMIGO_X_INICIAL + c * SEPARACION_HORIZONTAL;
                actual->y = ENEMIGO_Y_INICIAL + f;
                actual->activo = 1;
            }
        }
    }
    return enemigos;
}

void liberarEnemigos(Enemigo *enemigos) {
    free(enemigos);
}

void moverEnemigos(Enemigo *enemigos, int filas, int columnas, int *direccion) {
    Enemigo *actual = enemigos;
    Enemigo *fin = enemigos + filas * columnas;
    int tocaBorde = 0;

    while (actual < fin) {
        if (actual->activo) {
            int proximaX = actual->x + *direccion;
            if (proximaX <= 0 || proximaX >= ANCHO_TABLERO - 1) {
                tocaBorde = 1;
            }
        }
        actual++;
    }

    if (tocaBorde) {
        *direccion *= -1;
    }

    actual = enemigos;
    while (actual < fin) {
        if (actual->activo) {
            if (tocaBorde) {
                actual->y++;
            } else {
                actual->x += *direccion;
            }
        }
        actual++;
    }
}

int quedanEnemigosActivos(const Enemigo *enemigos, int filas, int columnas) {
    const Enemigo *actual = enemigos;
    const Enemigo *fin = enemigos + filas * columnas;
    int quedan = 0;

    while (actual < fin && quedan == 0) {
        if (actual->activo) {
            quedan = 1;
        }
        actual++;
    }
    return quedan;
}

int contarEnemigosActivos(const Enemigo *enemigos, int filas, int columnas) {
    const Enemigo *actual = enemigos;
    const Enemigo *fin = enemigos + filas * columnas;
    int contador = 0;

    while (actual < fin) {
        if (actual->activo) {
            contador++;
        }
        actual++;
    }
    return contador;
}

int enemigosLlegaronAlFondo(const Enemigo *enemigos, int filas, int columnas) {
    const Enemigo *actual = enemigos;
    const Enemigo *fin = enemigos + filas * columnas;
    int llegaron = 0;

    while (actual < fin && llegaron == 0) {
        if (actual->activo && actual->y >= ALTO_TABLERO - FILAS_LIBRES_SOBRE_JUGADOR) {
            llegaron = 1;
        }
        actual++;
    }
    return llegaron;
}

static int columnaTieneEnemigoActivo(const Enemigo *enemigos, int filas, int columnas, int columna) {
    int fila = 0;
    int encontrado = 0;

    while (fila < filas && encontrado == 0) {
        const Enemigo *actual = enemigos + fila * columnas + columna;
        if (actual->activo) {
            encontrado = 1;
        }
        fila++;
    }
    return encontrado;
}

int elegirColumnaConEnemigos(const Enemigo *enemigos, int filas, int columnas) {
    int columnasConEnemigos = 0;
    int objetivo;
    int columnaElegida = -1;
    int c;

    for (c = 0; c < columnas; c++) {
        if (columnaTieneEnemigoActivo(enemigos, filas, columnas, c)) {
            columnasConEnemigos++;
        }
    }

    if (columnasConEnemigos > 0) {
        objetivo = rand() % columnasConEnemigos;
        columnasConEnemigos = 0;
        for (c = 0; c < columnas && columnaElegida == -1; c++) {
            if (columnaTieneEnemigoActivo(enemigos, filas, columnas, c)) {
                if (columnasConEnemigos == objetivo) {
                    columnaElegida = c;
                }
                columnasConEnemigos++;
            }
        }
    }
    return columnaElegida;
}

Enemigo *enemigoMasBajoEnColumna(Enemigo *enemigos, int filas, int columnas, int columna)
{
    Enemigo *encontrado = NULL;
    int fila = filas - 1;

    if (columna >= 0 && columna < columnas)
    {
        while (fila >= 0 && encontrado == NULL)
        {
            Enemigo *actual = enemigos + fila * columnas + columna;

            if (actual->activo)
            {
                encontrado = actual;
            }
            fila--;
        }
    }
    return encontrado;
}
