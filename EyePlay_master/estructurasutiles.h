#ifndef ESTRUCTURASUTILES_H
#define ESTRUCTURASUTILES_H

#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

namespace  EstructuraUtiles {

    struct Circulo {
        Point centro;
        Point_<float> velocidad;
        int radio;
        bool isSelecionado;
    };

    struct Test {
        Point_<float> centro;
        Point_<float> velocidad;
        int radio;
        bool isSelecionado;
    };
}

#endif // ESTRUCTURASUTILES_H
