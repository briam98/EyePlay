#include "util.h"

const char* util::keys=
{
    "{help h | | show help message}{@camera_number| 0 | camera number}"
};

//TODO QUITAR c
//TODO la h no funciona
const string util::hot_keys =
    "\n\nAtajos de teclado: \n"
    "\tESC - quit the program\n"
    "\tc - stop the tracking\n"
    "\tb - switch to/from backprojection view\n"
    "\th - show/hide object histogram\n"
    "\tp - pause video\n"
    "To initialize tracking, select the object with mouse\n";

util::util()
{

}

void util::help()
{
    cout << "\nEye Play - Juego desarrollado por:\n"
            "           Briam Daniel Solano Godoy\n"
            "           Jose Antonio Pina Gomez\n"
            "para la asignatura de Informatica Grafica\n"
            "de 4º curso de Ingenieria Informatica en la Univesidad de Murcia\n";
    cout << util::hot_keys;
}

int util::juego(int juegoSeleccionado, Mat image, Circulo mando1, Circulo mando2, int frame) {
    switch(juegoSeleccionado) {
        case 1:
            return juegoRebote::iterar(image, mando1, mando2);
        case 0:
            return JuegoMusical::iterar(image, mando1, frame);
        default:
            return -1;
    }
}
