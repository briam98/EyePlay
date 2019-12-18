#ifndef EMISOR_H
#define EMISOR_H
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

using namespace cv;
using namespace std;

class Emisor
{
public:
    Emisor(Point_<float> centro, Mat imagen, Mat imagenMask, int numNota,
           int radio, int destino, bool colisionado);

    void deleteEmisor();

    Point_<float> getCentro();
    void setCentro(Point_<float> nuevoCentro);

    Point_<float> getIzqSuperior();

    Mat getImagen();
    void setImagen(Mat nuevaImagen);

    Mat getImagenMask();
    void setImagenMask(Mat nuevaImagenMask);

    int getRadio();
    void setRadio(int nuevoRadio);

    int getDestino();
    int getNum_Nota();

    bool isColisionado();
    void setColisionado(bool nuevoColisionado);
private:
    Point_<float> centro;
    int radio;
    int destino;
    Mat imagen;
    Mat imagenMask;
    int num_nota;
    bool colisionado;
};

#endif // EMISOR_H
