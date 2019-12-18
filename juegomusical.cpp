#include "juegomusical.h"
JuegoMusical::JuegoMusical()
{

}

const int NUM_RECEPTORES = 5;
const int NEGRO = 0;
const int ROJO = 1;
const int VERDE = 2;
const int AZUL = 3;
const int ORO = 4;

static Point dimensionesReceptor(70, 60);
static int radioReceptor = dimensionesReceptor.x / 2;
static Scalar colorReceptor(0,0,0);
static Rect arrayReceptores[NUM_RECEPTORES];
static Point centroReceptores[NUM_RECEPTORES];
static Point_<float> vUnitarioReceptores[NUM_RECEPTORES];

static int velocidadJuego = 5;   // Aumentar para que aumente la velocidad de los emisores
static int tiempoAparicion = 15; // Disminuir para que aumente la velocidad de aparicion

static list<Emisor> listasEmisores[NUM_RECEPTORES];

static int radioEmisor = 25;
static int tamEmisor = radioEmisor*2;
static Scalar colorEmisor(0,0,255);
static Point centroInicioEmisor;

static Mat tambor;
static Mat mask_tambor;
static Mat notas[4][5];
static Mat mask_notas[4][5];
static string sonidoNotas[4];

static Mat bigImage;
static Mat bigImageRoi;

static bool primeraIteracion = true;

static list<puntuacion> listaPuntuacion;
static int puntosGlobales = 0;
static double tamLetraInicial = 1;
void JuegoMusical::addPuntos(Emisor* emisor, int puntos) {

    Scalar color;
    if (puntos < 50) {
        color = Scalar(0,0,255);
        emisor->setImagen(notas[emisor->getNum_Nota()][ROJO]);
        emisor->setImagenMask(mask_notas[emisor->getNum_Nota()][ROJO]);
    } else if (puntos >= 50 && puntos < 75) {
        color = Scalar(255,0,0);
        emisor->setImagen(notas[emisor->getNum_Nota()][AZUL]);
        emisor->setImagenMask(mask_notas[emisor->getNum_Nota()][AZUL]);
    } else if (puntos >= 75 && puntos < 100) {
        color = Scalar(0,255,0);
        emisor->setImagen(notas[emisor->getNum_Nota()][VERDE]);
        emisor->setImagenMask(mask_notas[emisor->getNum_Nota()][VERDE]);
    } else {
        color = Scalar(55,175,212);
        emisor->setImagen(notas[emisor->getNum_Nota()][ORO]);
        emisor->setImagenMask(mask_notas[emisor->getNum_Nota()][ORO]);
    }
    Point centroReceptor = centroReceptores[emisor->getDestino()];
    puntuacion aux;
    if (emisor->getDestino() == 2) {
        aux = puntuacion(Point(centroReceptor.x - radioReceptor - 20, centroReceptor.y + radioReceptor + 20),
                         puntos, tamLetraInicial, color);
    } else {
        aux = puntuacion(Point(centroReceptor.x - radioReceptor - 20, centroReceptor.y - radioReceptor - 20),
                         puntos, tamLetraInicial, color);
    }

    listaPuntuacion.push_back(aux);
    puntosGlobales += puntos;
}

void JuegoMusical::mostrarPuntos() {
    list<puntuacion>::iterator it = listaPuntuacion.begin();

    while (it != listaPuntuacion.end()) {
        String stringPuntos = to_string((*it).puntos);
        putText(bigImage, stringPuntos, (*it).posicion,
                FONT_HERSHEY_SCRIPT_SIMPLEX, (*it).tamLetra, (*it).color, 2, LINE_AA);
        (*it).disminuirTam();
        if ((*it).tamLetra < 0.5) {
            it = listaPuntuacion.erase(it);  // alternatively, i = items.erase(i);
        }
        else {
            ++it;
        }
    }
}

void JuegoMusical::colisiones(Circulo mando) {
    Point centroMando = Point(static_cast<int>(mando.centro.x) + 50, static_cast<int>(mando.centro.y + 50));
    circle(bigImage, centroMando, 2, Scalar(0,0,0), 2, LINE_AA, 0);
    for (int i = 0; i < NUM_RECEPTORES; i++) {
        int distancia = static_cast<int>(sqrt(pow(centroReceptores[i].x - centroMando.x, 2) +
                        pow(centroReceptores[i].y - centroMando.y, 2)));

        if (distancia <= radioReceptor + mando.radio) {
            //Si esta colisionando comprobar si ese receptor esta colisionando con algun emisor
            for (list<Emisor>::iterator it=listasEmisores[i].begin(); it != listasEmisores[i].end(); ++it) {
                if (!(*it).isColisionado()) {
                    Point centroEmisor = Point(static_cast<int>((*it).getCentro().x), static_cast<int>((*it).getCentro().y));
                    int distancia2 = static_cast<int>(sqrt(pow(centroReceptores[i].x - centroEmisor.x, 2) +
                                    pow(centroReceptores[i].y - centroEmisor.y, 2)));
                    if (distancia2 <= (*it).getRadio() + radioReceptor) {
                        addPuntos(&(*it), 100-distancia2+1);
                        (*it).setColisionado(true);
                        return;
                    }
                }
            }
        }
    }
}

void JuegoMusical::moverEmisores() {
    for (int i = 0; i < NUM_RECEPTORES; i++) {
        bool borrarPrimero = false;
        for (list<Emisor>::iterator it=listasEmisores[i].begin(); it != listasEmisores[i].end(); it++) {

            Point_<float> centroEmisor = (*it).getCentro();

            float nuevaX = centroEmisor.x - vUnitarioReceptores[i].x * velocidadJuego;
            float nuevaY = centroEmisor.y - vUnitarioReceptores[i].y * velocidadJuego;

            (*it).setCentro(Point_<float>(nuevaX, nuevaY));

            if (centroEmisor.y <= 0 || centroEmisor.x <= 0) {
                borrarPrimero = true;
            }
        }
        if (borrarPrimero) {
            Emisor aux = listasEmisores[i].front();
            listasEmisores[i].pop_front();
            aux.deleteEmisor();
        }
    }
}

void JuegoMusical::inicializarValores(Mat image) {
    srand(static_cast<unsigned int>(time(nullptr)));
    //Inicializar listas de emisores
    for (int i = 0; i < NUM_RECEPTORES; i++) {
        list<Emisor> aux;
        listasEmisores[i] = aux;
    }

    bigImage = Mat(image.rows+tamEmisor,image.cols+2*tamEmisor, CV_8UC3);
    bigImageRoi = Mat(bigImage, Rect(tamEmisor,tamEmisor,image.cols,image.rows));

    centroInicioEmisor = Point(bigImage.cols / 2, 500);

    tambor = imread("..\\EyePlay_master\\resources\\tambor1.png");

    inicializarImagenes();

    cvtColor(tambor, mask_tambor, COLOR_BGR2GRAY);

    threshold(mask_tambor, mask_tambor, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

    centroReceptores[0] = Point(110, 260);
    centroReceptores[1] = Point(180, 140);
    centroReceptores[2] = Point(370, 100);
    centroReceptores[3] = Point(560, 140);
    centroReceptores[4] = Point(625, 260);

    for (int i = 0; i < NUM_RECEPTORES; i++) {
        Point puntoRectangulo(centroReceptores[i].x - 30, centroReceptores[i].y - 30);
        arrayReceptores[i] = Rect(puntoRectangulo.x, puntoRectangulo.y, dimensionesReceptor.x, dimensionesReceptor.y);
        Point diferenciaCentros(centroInicioEmisor.x - centroReceptores[i].x, centroInicioEmisor.y - centroReceptores[i].y);
        double modulo = sqrt(diferenciaCentros.x*diferenciaCentros.x + diferenciaCentros.y*diferenciaCentros.y);
        float unitarioX = diferenciaCentros.x / static_cast<float>(modulo);
        float unitarioY = diferenciaCentros.y / static_cast<float>(modulo);
        vUnitarioReceptores[i] = Point_<float>(unitarioX, unitarioY);
    }

}

void JuegoMusical::iterar(Mat image, Circulo mando, int frame) {
    if (primeraIteracion) {
        inicializarValores(image);
        primeraIteracion = false;
    }
    if (frame % tiempoAparicion == 0) {
        int destino = rand() % 5;
        int num_imagen = rand() % 4;
        Emisor nuevoEmisor(centroInicioEmisor,
                           notas[num_imagen][NEGRO], mask_notas[num_imagen][NEGRO],
                           num_imagen, radioEmisor, destino, false);
        listasEmisores[destino].push_back(nuevoEmisor);
    }

    //Copiamos el frame actual a uno más grande
    image.copyTo(bigImageRoi);

    String stringPuntos = "Puntuacion: " + to_string(puntosGlobales);
    putText(bigImage, stringPuntos, Point(450,510),
            FONT_HERSHEY_DUPLEX, 0.8, Scalar(255,255,255), 1, LINE_AA);

    //Pintamos los emisores en la imagen mas grande
    for (int i = 0; i < NUM_RECEPTORES; i++) {
        Mat destinationRoi(bigImage, arrayReceptores[i]);
        tambor.copyTo(destinationRoi, mask_tambor);
        circle(bigImage, centroReceptores[i], 2, colorReceptor, 1, LINE_AA, 0);

        for (list<Emisor>::iterator it=listasEmisores[i].begin(); it != listasEmisores[i].end(); ++it) {
            int px = static_cast<int>((*it).getIzqSuperior().x);
            int py = static_cast<int>((*it).getIzqSuperior().y);
            Rect rectDibujo;

            if (px > 0 && py > 0 && px < bigImage.cols-tamEmisor) {
                rectDibujo = Rect(px, py, (*it).getRadio()*2, (*it).getRadio()*2);

                Mat destinationRoiEmisor(bigImage, rectDibujo);
                (*it).getImagen().copyTo(destinationRoiEmisor, (*it).getImagenMask());
            }
        }
    }

    colisiones(mando);
    moverEmisores();
    mostrarPuntos();

    //Copiamos la parte que nos interesa de la bigImagen a imagen
    Mat imageRoi(image, Rect(0,0,image.cols,image.rows));
    bigImageRoi.copyTo(imageRoi);
}

void JuegoMusical::inicializarImagenes() {
    notas[0][0] = imread("..\\EyePlay_master\\resources\\nota0.png");
    notas[0][1] = imread("..\\EyePlay_master\\resources\\nota0_rojo.png");
    notas[0][2] = imread("..\\EyePlay_master\\resources\\nota0_verde.png");
    notas[0][3] = imread("..\\EyePlay_master\\resources\\nota0_azul.png");
    notas[0][4] = imread("..\\EyePlay_master\\resources\\nota0_oro.png");

    notas[1][0] = imread("..\\EyePlay_master\\resources\\nota1.png");
    notas[1][1] = imread("..\\EyePlay_master\\resources\\nota1_rojo.png");
    notas[1][2] = imread("..\\EyePlay_master\\resources\\nota1_verde.png");
    notas[1][3] = imread("..\\EyePlay_master\\resources\\nota1_azul.png");
    notas[1][4] = imread("..\\EyePlay_master\\resources\\nota1_oro.png");

    notas[2][0] = imread("..\\EyePlay_master\\resources\\nota2.png");
    notas[2][1] = imread("..\\EyePlay_master\\resources\\nota2_rojo.png");
    notas[2][2] = imread("..\\EyePlay_master\\resources\\nota2_verde.png");
    notas[2][3] = imread("..\\EyePlay_master\\resources\\nota2_azul.png");
    notas[2][4] = imread("..\\EyePlay_master\\resources\\nota2_oro.png");

    notas[3][0] = imread("..\\EyePlay_master\\resources\\nota3.png");
    notas[3][1] = imread("..\\EyePlay_master\\resources\\nota3_rojo.png");
    notas[3][2] = imread("..\\EyePlay_master\\resources\\nota3_verde.png");
    notas[3][3] = imread("..\\EyePlay_master\\resources\\nota3_azul.png");
    notas[3][4] = imread("..\\EyePlay_master\\resources\\nota3_oro.png");

    for (int i = 0; i < 4; i++) {
        for (int j = 0 ; j < 5; j++) {
            cvtColor(notas[i][j], mask_notas[i][j], COLOR_BGR2GRAY);
            threshold(mask_notas[i][j], mask_notas[i][j], 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
        }
    }
}
