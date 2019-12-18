#include "util.h"

const char* util::keys=
{
    "{help h | | show help message}{@camera_number| 0 | camera number}"
};

const string util::hot_keys =
    "\n\nHot keys: \n"
    "\tESC - quit the program\n"
    "\tc - stop the tracking\n"
    "\tb - switch to/from backprojection view\n"
    "\th - show/hide object histogram\n"
    "\tp - pause video\n"
    "\t1 - Juego 1\n"
    "\t2 - Juego 2\n"
    "To initialize tracking, select the object with mouse\n";

util::util()
{

}

void util::help()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects such as your face and it tracks it.\n"
            "This reads from video camera (0 by default, or the camera number the user enters\n"
            "Usage: \n"
            "   ./camshiftdemo [camera number]\n";
    cout << util::hot_keys;
}

int util::juego(int juegoSeleccionado, Mat image, Circulo mando1, Circulo mando2, int frame) {
    switch(juegoSeleccionado) {
        case 1:
            return juegoRebote::iterar(image, mando1, mando2);
            break;
        case 0:
            JuegoMusical::iterar(image, mando1, frame);
            return 0;
            break;
    }
}
