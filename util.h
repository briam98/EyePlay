#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <iostream>
#include <juegorebote.h>
#include <juegomusical.h>
using namespace std;


class util
{
public:
    util();
    static void help();
    static const char* keys;
    static const string hot_keys;
    static int juego(int juegoSeleccionado, Mat image, Circulo mando1, Circulo mando2, int frame);
};

#endif // UTIL_H
