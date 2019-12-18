#ifndef JUEGOREBOTE_H
#define JUEGOREBOTE_H
#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctype.h>
#include "estructurasutiles.h"
#include <math.h>

using namespace cv;
using namespace std;
using namespace EstructuraUtiles;


class juegoRebote
{
public:
    juegoRebote();
    static int iterar(Mat imagen, Circulo mando1, Circulo mando2);
private:
    static void calcularColisionesBordes(Mat imagen);
    static void calcularColisionesMando(Circulo mando);
    static void desplazarBola(Circulo mando, float distancia);
    static void moverBola();

    static void calcularNuevaVelocidad(Circulo mando);


    static void mostrarInfoJuego(Mat imagen);
    static void cargarSeleccion(Circulo mando, Rect boton);
    static void dibujarBarraCarga(Mat imagen);

    static void dibujarBalon(Mat imagen);
    static void mostrarMensajeGol(Mat imagen);
    static void mostrarTiempo(Mat imagen);
    static void dibujarMarcador(Mat imagen);

    static void inicializar(Mat imagen);
    static void inicializarImagenes();
    static void mostarCelebracion(Mat imagen);
    static void mostrarBotonSalir(Mat imagen);
};

#endif // JUEGOREBOTE_H
