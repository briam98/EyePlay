#include "emisor.h"

Emisor::Emisor(Point_<float> centro, Mat imagen, Mat imagenMask, int num_nota,
               int radio, int destino, bool colisionado)
{
    this->centro = centro;
    this->radio = radio;
    this->destino = destino;
    this->imagen = imagen;
    this->num_nota = num_nota;
    this->imagenMask = imagenMask;
    this->colisionado = colisionado;
}


void Emisor::deleteEmisor() {
    delete(this);
}

Point_<float> Emisor::getCentro() {
    return this->centro;
}

void Emisor::setCentro(Point_<float> nuevoCentro) {
    this->centro = nuevoCentro;
}

Point_<float> Emisor::getIzqSuperior() {
    return Point_<float>(centro.x - radio, centro.y - radio);
}

int Emisor::getRadio() {
    return this->radio;
}

void Emisor::setRadio(int nuevoRadio) {
    this->radio = nuevoRadio;
}

int Emisor::getDestino() {
    return this->destino;
}

Mat Emisor::getImagen() {
    return this->imagen;
}

void Emisor::setImagen(Mat nuevaImagen) {
    this->imagen = nuevaImagen;
}

Mat Emisor::getImagenMask() {
    return this->imagenMask;
}

void Emisor::setImagenMask(Mat nuevaImagenMask) {
    this->imagenMask = nuevaImagenMask;
}

int Emisor::getNum_Nota() {
    return this->num_nota;
}

bool Emisor::isColisionado() {
    return this->colisionado;
}

void Emisor::setColisionado(bool nuevoColisionado) {
    this->colisionado = nuevoColisionado;
}
