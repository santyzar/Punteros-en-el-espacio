#include "juego.h"
#include "colisiones.h"
#include <stdlib.h>

void inicializarJuego(Juego *juego) {
    inicializarJugador(&juego->jugador);

    juego->filasEnemigos = FILAS_ENEMIGOS;
    juego->columnasEnemigos = COLUMNAS_ENEMIGOS;
    juego->enemigos = crearEnemigos(juego->filasEnemigos, juego->columnasEnemigos);
    juego->direccionEnemigos = 1;

    inicializarDisparos(juego->disparos, MAX_DISPAROS);

    juego->puntaje = 0;
    juego->jugando = 1;

    if (juego->enemigos == NULL) {
        juego->jugando = 0;
    }
    juego->gano = 0;

    juego->nivel = 1;
    juego->velocidadEnemigos = FRAMES_POR_AVANCE;
    juego->velocidadDisparoEnemigo = FRAMES_POR_DISPARO_ENEMIGO;

    inicializarEscudos(juego->escudos, CANTIDAD_ESCUDOS);
}

void dispararJuego(Juego *juego)
{
    if (juego->jugando != 0 && juego->enemigos != NULL)
        dispararJugador(juego->disparos, MAX_DISPAROS,
                        juego->jugador.x, juego->jugador.y);
}

void reiniciarJuego(Juego *juego)
{
    liberarJuego(juego);
    inicializarJuego(juego);
}

void dispararEnemigoAleatorio(Juego *juego)
{
    int columna = elegirColumnaConEnemigos(juego->enemigos, juego->filasEnemigos, juego->columnasEnemigos);
    Enemigo *tirador;

    if (columna != -1) {
        tirador = enemigoMasBajoEnColumna(juego->enemigos, juego->filasEnemigos,
                                          juego->columnasEnemigos, columna);
        if (tirador != NULL)
            dispararEnemigo(juego->disparos, MAX_DISPAROS, tirador->x, tirador->y);
    }
}

void moverYDispararEnemigos(Juego *juego, int contadorFrames)
{
    int enemigosActivos = contarEnemigosActivos(juego->enemigos, juego->filasEnemigos, juego->columnasEnemigos);
    int intervaloMovimientoEnemigos = juego->velocidadEnemigos;
    int intervaloDisparoEnemigo = juego->velocidadDisparoEnemigo;

    if (enemigosActivos == 1)
    {
        if (intervaloMovimientoEnemigos < FRAMES_POR_AVANCE_ULTIMO_ENEMIGO)
            intervaloMovimientoEnemigos = FRAMES_POR_AVANCE_ULTIMO_ENEMIGO;
        if (intervaloDisparoEnemigo < FRAMES_POR_DISPARO_ULTIMO_ENEMIGO)
            intervaloDisparoEnemigo = FRAMES_POR_DISPARO_ULTIMO_ENEMIGO;
    }

    if (contadorFrames % intervaloMovimientoEnemigos == 0)
        moverEnemigos(juego->enemigos, juego->filasEnemigos,
                      juego->columnasEnemigos, &juego->direccionEnemigos);

    if (contadorFrames % intervaloDisparoEnemigo == 0)
        dispararEnemigoAleatorio(juego);
}

void pasarAlSiguienteNivel(Juego *juego)
{
    juego->nivel++;
    if (juego->velocidadEnemigos > MIN_INTERVALO_MOVIMIENTO_ENEMIGOS)
        juego->velocidadEnemigos--;
    if (juego->velocidadDisparoEnemigo > MIN_INTERVALO_DISPARO_ENEMIGO)
        juego->velocidadDisparoEnemigo--;

    inicializarDisparos(juego->disparos, MAX_DISPAROS);
    liberarEnemigos(juego->enemigos);
    juego->enemigos = crearEnemigos(juego->filasEnemigos, juego->columnasEnemigos);
    juego->direccionEnemigos = 1;

    inicializarEscudos(juego->escudos, CANTIDAD_ESCUDOS);
    if (juego->enemigos == NULL) {
        juego->jugando = 0;
    }

}

void resolverFinDeNivel(Juego *juego)
{
    if (quedanEnemigosActivos(juego->enemigos, juego->filasEnemigos, juego->columnasEnemigos) == 0) {
        if (juego->nivel >= NIVEL_VICTORIA) {
            juego->jugando = 0;
            juego->gano = 1;
        } else {
            pasarAlSiguienteNivel(juego);
        }
    } else if (enemigosLlegaronAlFondo(juego->enemigos, juego->filasEnemigos, juego->columnasEnemigos)) {
        juego->jugando = 0;
        juego->gano = 0;
    }
}

void actualizarJuego(Juego *juego, int contadorFrames) {
    if (juego->jugando != 0 && juego->enemigos != NULL) {
        moverDisparos(juego->disparos, MAX_DISPAROS);

        revisarColisionesPorDisparos(juego->disparos, MAX_DISPAROS);
        revisarColisionesDisparosEscudos(juego->disparos, MAX_DISPAROS, juego->escudos, CANTIDAD_ESCUDOS);
        revisarColisionesDisparosEnemigos(juego->disparos, MAX_DISPAROS,
                                          juego->enemigos, juego->filasEnemigos,
                                          juego->columnasEnemigos, &juego->puntaje);
        revisarColisionesDisparosJugador(juego->disparos, MAX_DISPAROS, &juego->jugador, &juego->jugando);

        if (juego->jugando != 0) {
            moverYDispararEnemigos(juego, contadorFrames);
            resolverFinDeNivel(juego);
        }
    }
}

void liberarJuego(Juego *juego) {
    liberarEnemigos(juego->enemigos);

    juego->enemigos = NULL;
    juego->jugando = 0;
}
