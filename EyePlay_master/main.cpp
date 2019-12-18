#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctype.h>
#include "util.h"
#include "menu.h"

using namespace cv;
using namespace std;

static Mat image;
static int num_frame = 0;
static const int RESET_NUM_FRAME = 10000;

static bool backprojMode = false;
static int selectObject = 0;
static int trackObject = 0;
static bool showHist = true;
static Point origin;
static Rect selection1;
static Rect selection2;
static int vmin = 10, vmax = 256, smin = 30;
static Circulo mando1;
static Circulo mando2;
static bool confMode = true;
static bool isFirstTime = true;
static bool isFirstTimeMando2 = true;
static Point anteriorCentro;
static Point anteriorCentro2;

static bool mandoUnoSeleccionado = false;
static bool mandoDosSeleccionado = false;

//VARIABLES PARA TEST
static bool debugMode = true;

static int v = 0;


// User draws box around object to track. This triggers CAMShift to start tracking
static void onMouse( int event, int x, int y, int, void* )
{
    if (confMode) {
        if( selectObject == 1 && !mando1.isSelecionado)
	    {
	        selection1.x = MIN(x, origin.x);
	        selection1.y = MIN(y, origin.y);
	        selection1.width = std::abs(x - origin.x);
	        selection1.height = std::abs(y - origin.y);

	        selection1 &= Rect(0, 0, image.cols, image.rows);
	    }
        if (selectObject == 2 && !mando2.isSelecionado)
	    {
	        selection2.x = MIN(x, origin.x);
	        selection2.y = MIN(y, origin.y);
	        selection2.width = std::abs(x - origin.x);
	        selection2.height = std::abs(y - origin.y);

	        selection2 &= Rect(0, 0, image.cols, image.rows);
	    }

	    switch( event )
	    {
	    case EVENT_LBUTTONDOWN:
	        origin = Point(x,y);
	        selection1 = Rect(x,y,0,0);
	        selectObject = 1;
	        break;
	    case EVENT_LBUTTONUP:
	        selectObject = 0;
	        if( selection1.width > 0 && selection1.height > 0 )
	            trackObject = -1;   // Set up CAMShift properties in main() loop
	        break;
	    case EVENT_RBUTTONDOWN:
	        origin = Point(x,y);
	        selection2 = Rect(x,y,0,0);
	        selectObject = 2;
	        break;
	    case EVENT_RBUTTONUP:
	        selectObject = 0;
	        if( selection2.width > 0 && selection2.height > 0 )
	            trackObject = -2;   // Set up CAMShift properties in main() loop
	        break;
	    }
	}
}
void mostrarPantallaJuego() {
    destroyWindow("Configuracion mando");

    namedWindow( "Juego", WINDOW_AUTOSIZE | WINDOW_KEEPRATIO | WINDOW_GUI_NORMAL);
    imshow("Juego", image);
}

void mostrarConfiguracionMando() {

    if (v == -1) {
        destroyWindow("Juego");

        namedWindow( "Configuracion mando", WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

        createTrackbar( "Vmin", "Configuracion mando", &vmin, 256, nullptr );
        createTrackbar( "Vmax", "Configuracion mando", &vmax, 256, nullptr );
        createTrackbar( "Smin", "Configuracion mando", &smin, 256, nullptr );

        setMouseCallback( "Configuracion mando", onMouse, nullptr );
    }


    imshow("Configuracion mando", image);
}
int main()
{
    VideoCapture cap;
    Rect trackWindow1, trackWindow2;
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;
    util::help();
    int camNum = 0;
    cap.open(camNum);

    if( !cap.isOpened() )
    {
        cout << "***Could not initialize capturing...***\n";
        return -1;
    }
   // cout << util::hot_keys;

    namedWindow( "Configuracion mando", WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

    createTrackbar( "Vmin", "Configuracion mando", &vmin, 256, nullptr );
    createTrackbar( "Vmax", "Configuracion mando", &vmax, 256, nullptr );
    createTrackbar( "Smin", "Configuracion mando", &smin, 256, nullptr );

    setMouseCallback( "Configuracion mando", onMouse, nullptr );
	
    Mat frame, hsv, hue, mask, hist1, hist2, histimg1 = Mat::zeros(200, 320, CV_8UC3);
    Mat histimg2 = Mat::zeros(200, 320, CV_8UC3);
    Mat backproj1, backproj2;
    bool paused = false;

    cap.set(CAP_PROP_FPS, 60);
    for(;;)
    {
        if( !paused )
        {
            cap >> frame;
            if( frame.empty() )
                break;
        }

        frame.copyTo(image);
        flip(image, image, 1);

        num_frame++;
        if (num_frame == RESET_NUM_FRAME) {
            num_frame = 0;
        }

        if( !paused )
        {
            //Esto convierte la imagen de RGB a HSV y lo guarda en hsv
            cvtColor(image, hsv, COLOR_BGR2HSV);
            //imshow("HSV", hsv);

            //Ask the user to select a target to be track*/
            if(trackObject)
            {
                int _vmin = vmin, _vmax = vmax;

                //En mask tenemos la imagen completa en la que el objeto seleccionado aparece en blanco y el resto debe aparecer en negro
                //Esto se hace con la funcion inRange y los valores vmin, vmax, smin.
                //Los pixeles que se muestran en blanco en mask tienen un valor entre vmin y vmax y una saturacion superior? a smin
                inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                //imshow("mask", mask);
                int ch[] = {0, 0};

                //A partir de la imagen en HSV se calcula el matiz (hue)
                hue.create(hsv.size(), hsv.depth());

                //QUIZA: rellena hue a partir de hsv
                mixChannels(&hsv, 1, &hue, 1, ch, 1);
                //imshow("HUE", hue);


                if( trackObject == -1)
                {
                    mandoUnoSeleccionado = true;
                    // Object has been selected by user, set up CAMShift search properties once
                    Mat roi(hue, selection1); //El roi es la región seleccionada del hue

                    Mat maskroi(mask, selection1); //Maskroi esta vacio
                    //imshow("roi", roi);
                    //imshow("maskroi", maskroi);
                    //Calcula el histrograma de la parte seleccionada a partir de la mascara y lo guarda en hist
                    calcHist(&roi, 1, nullptr, maskroi, hist1, 1, &hsize, &phranges);
                    //Normaliza los colores de la parte seleccionada dentro del rango de colores de HSV
                    normalize(hist1, hist1, 0, 255, NORM_MINMAX);

                    trackWindow1 = selection1; //TrackWindow empieza con las dimensiones del cuadrado seleccionado
                    //Se pone a 1 para no volver a entrar a esta parte del codigo hasta que el usuario haga otra seleccion
                    trackObject = 1; // Don't set up again, unless user selects new ROI

                    //Esta parte del codigo solo sirve para crear y mostrar la ventana del histograma
                    histimg1 = Scalar::all(0);
                    int binW = histimg1.cols / hsize;
                    Mat buf(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(buf, buf, COLOR_HSV2BGR);

                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(hist1.at<float>(i)*histimg1.rows/255);
                        rectangle( histimg1, Point(i*binW,histimg1.rows),
                                   Point((i+1)*binW,histimg1.rows - val),
                                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
                    }
                }

                if( trackObject == -2)
                {
                    mandoDosSeleccionado = true;
                    // Object has been selected by user, set up CAMShift search properties once
                    Mat roi(hue, selection2); //El roi es la región seleccionada del hue

                    Mat maskroi(mask, selection2); //Maskroi esta vacio
                    //imshow("roi", roi);
                    //imshow("maskroi", maskroi);
                    //Calcula el histrograma de la parte seleccionada a partir de la mascara y lo guarda en hist
                    calcHist(&roi, 1, nullptr, maskroi, hist2, 1, &hsize, &phranges);
                    //Normaliza los colores de la parte seleccionada dentro del rango de colores de HSV
                    normalize(hist2, hist2, 0, 255, NORM_MINMAX);

                    trackWindow2 = selection2; //TrackWindow empieza con las dimensiones del cuadrado seleccionado
                    //Se pone a 1 para no volver a entrar a esta parte del codigo hasta que el usuario haga otra seleccion
                    trackObject = 1; // Don't set up again, unless user selects new ROI

                    //Esta parte del codigo solo sirve para crear y mostrar la ventana del histograma
                    histimg2 = Scalar::all(0);
                    int binW = histimg2.cols / hsize;
                    Mat buf(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(buf, buf, COLOR_HSV2BGR);

                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(hist2.at<float>(i)*histimg2.rows/255);
                        rectangle( histimg2, Point(i*binW,histimg2.rows),
                                   Point((i+1)*binW,histimg2.rows - val),
                                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
                    }
                }

                // Perform CAMShift
                if (mandoUnoSeleccionado) {
                    calcBackProject(&hue, 1, nullptr, hist1, backproj1, &phranges);

                    backproj1 &= mask;

                    //Aqui se llama a la funcion que hace el seguimiento del objeto seleccionado.
                    //Nos devuelve un rectangulo rotado a partir del cual crearemos el circulo
                    RotatedRect trackBox = CamShift(backproj1, trackWindow1,
                                        TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));

	               // Seteamos los valores del nuevo circulo del mando
                   mando1.radio =  static_cast<int> MIN(trackBox.size.width, trackBox.size.height) / 2;
                   mando1.centro = trackBox.center;

	               if (isFirstTime) {

	                    isFirstTime = false;
	                    anteriorCentro.x = static_cast<int>(trackBox.center.x);
	                    anteriorCentro.y = static_cast<int>(trackBox.center.y);
	               } else {

                       mando1.velocidad.x = mando1.centro.x - anteriorCentro.x;
                       mando1.velocidad.y = mando1.centro.y - anteriorCentro.y;
	               }

                    //Esto no sabemos cuando se ejecuta
                    if( trackWindow1.area() <= 1 )
                    {
                        int cols = backproj1.cols, rows = backproj1.rows, r = (MIN(cols, rows) + 5)/6;
                        trackWindow1 = Rect(trackWindow1.x - r, trackWindow1.y - r,
                                           trackWindow1.x + r, trackWindow1.y + r) &
                                      Rect(0, 0, cols, rows);
                        cout << "EJECUTA" << endl;
                    }

                    if( backprojMode ) //Si tienes puesto el modo backproject, cambia la imagen por el backproject
                        cvtColor( backproj1, image, COLOR_GRAY2BGR );

                    //Crea el circulo del mando a partir del trackBox y la incluye a la imagen
					
					// TODO
                    //circle(image, trackBox.center, radio, Scalar(0,0,255), 3, LINE_AA, 0);

                }
                if (mandoDosSeleccionado) {
                    calcBackProject(&hue, 1, nullptr, hist2, backproj2, &phranges);

                    backproj2 &= mask;

                    //Aqui se llama a la funcion que hace el seguimiento del objeto seleccionado.
                    //Nos devuelve un rectangulo rotado a partir del cual crearemos la elipse
                    RotatedRect trackBox = CamShift(backproj2, trackWindow2,
                                        TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));

            		// TODO
                    //Seteamos los valores del nuevo circulo del mando
                   mando2.radio =  static_cast<int> MIN(trackBox.size.width, trackBox.size.height) / 2;
                   mando2.centro = trackBox.center;

                   if (isFirstTimeMando2) {

                        isFirstTimeMando2 = false;
                        anteriorCentro2.x = static_cast<int>(trackBox.center.x);
                        anteriorCentro2.y = static_cast<int>(trackBox.center.y);
                   } else {

                       mando2.velocidad.x = mando2.centro.x - anteriorCentro2.x;
                       mando2.velocidad.y = mando2.centro.y - anteriorCentro2.y;
                   }


                    //Esto no sabemos cuando se ejecuta
                    if( trackWindow2.area() <= 1 )
                    {
                        int cols = backproj2.cols, rows = backproj2.rows, r = (MIN(cols, rows) + 5)/6;
                        trackWindow2 = Rect(trackWindow2.x - r, trackWindow2.y - r,
                                           trackWindow2.x + r, trackWindow2.y + r) &
                                      Rect(0, 0, cols, rows);
                    }

                    if( backprojMode ) //Si tienes puesto el modo backproject, cambia la imagen por el backproject
                        cvtColor( backproj2, image, COLOR_GRAY2BGR );

                    //Crea el circulo del mando a partir del trackBox y la incluye a la imagen
                    // circle(image, trackBox.center, radio, Scalar(255,0,0), 3, LINE_AA, 0);
                }
            }
        }
        else if( trackObject < 0 )
            paused = false;

        if( selectObject && selection1.width > 0 && selection1.height > 0 )
        {
            Mat roi(image, selection1);
            bitwise_not(roi, roi);
        }

        if( selectObject && selection2.width > 0 && selection2.height > 0 )
        {
            Mat roi(image, selection2);
            bitwise_not(roi, roi);
        }

		if (confMode) {
            if (mandoUnoSeleccionado) {
                circle(image, mando1.centro, mando1.radio, Scalar(0,0,255), 3, LINE_AA, 0);
            }

            if (mandoDosSeleccionado) {
                circle(image, mando2.centro, mando2.radio, Scalar(255,0,0), 3, LINE_AA, 0);
            }
            if (!mandoUnoSeleccionado) {
                putText(image, "Selecciona con el click izquierdo el mando 1", Point(80, image.rows - 20 - 1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2, FILLED);
            } else if (!mandoDosSeleccionado && v == -1) {
                putText(image, "Selecciona con el click derecho el mando 2", Point(80, image.rows - 20 - 1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2, FILLED);
            }
            mostrarConfiguracionMando();
        } else {
            v = Menu::iterarMenu(image, mando1, mando2, num_frame);
            if (mandoUnoSeleccionado) {
                circle(image, mando1.centro, mando1.radio, Scalar(0,0,255), 3, LINE_AA, 0);
            }

            if (mandoDosSeleccionado) {
                circle(image, mando2.centro, mando2.radio, Scalar(255, 0, 0), 3, LINE_AA, 0);
            }
            mostrarPantallaJuego();
        }

        if (v == -1 && !mando2.isSelecionado) {
            confMode = true;
        }

        char c = static_cast<char>(waitKey(10));
        if( c == 27 )
            break;
        switch(c)
        {
        case 'b':
            backprojMode = !backprojMode;
            break;
        case 'c':
            trackObject = 0;
            histimg1 = Scalar::all(0);
            histimg2 = Scalar::all(0);
            break;
        case 'h':
			// TODO
            showHist = !showHist;
            if ( !showHist ) {
                destroyWindow( "Histogram1" );
                destroyWindow( "Histogram2" );
            } else {
                namedWindow( "Histogram1", 1 );
                namedWindow( "Histogram2", 1 );
            }
            break;
        case 'p':
            paused = !paused;
            break;
        case 32:
            if (mandoUnoSeleccionado && v != -1) {
                confMode = false;
                mando1.isSelecionado = true;
            }

            if (mandoUnoSeleccionado && mandoDosSeleccionado) {
                confMode = false;
                mando2.isSelecionado = true;
            }
            break;
        default:
            ;
        }
    }

    return 0;
}
