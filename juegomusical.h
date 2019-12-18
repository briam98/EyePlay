#ifndef JUEGOMUSICAL_H
#define JUEGOMUSICAL_H
#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctype.h>
#include <ctime>
#include <list>
#include <math.h>
#include "emisor.h"
#include "estructurasutiles.h"

using namespace cv;
using namespace std;
using namespace EstructuraUtiles;

struct puntuacion {
    Point posicion;
    int puntos;
    double tamLetra;
    Scalar color;

    puntuacion(Point posicion, int puntos, double tamLetra, Scalar color) {
        this->posicion = posicion;
        this->puntos = puntos;
        this->tamLetra = tamLetra;
        this->color = color;
    }
    puntuacion() {}
public: void disminuirTam() {
        tamLetra -= 0.05;
    }
};

class JuegoMusical
{
public:
    JuegoMusical();
    static void iterar(Mat image, Circulo mando, int frame);
private:
    static void addPuntos(Emisor* emisor, int puntos);
    static void mostrarPuntos();
    static void colisiones(Circulo mando);
    static void moverEmisores();
    static void inicializarValores(Mat image);
    static void inicializarImagenes();
};

#endif // JUEGOMUSICAL_H
