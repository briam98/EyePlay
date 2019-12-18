#ifndef MENU_H
#define MENU_H

#include <opencv2/opencv.hpp>
#include "util.h"
#include <list>

using namespace cv;


class Menu
{
public:
    Menu();
    static int iterarMenu(Mat imagen, Circulo mando1, Circulo mando2, int nFrame);

private:
    static void dibujarMenu(Mat imagen);
    static void cargarSeleccion(Circulo mando);
    static void dibujarBarraCarga(Mat imagen);
};

#endif // MENU_H
