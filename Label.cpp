#include "Label.h"
#include <string>
#include <iostream>
using std::string;

Label::Label(QPixmap& terrenos, string id, string tipo, int x, int y, 
    QWidget* parent) :  QLabel(parent), id(id), x(x), y(y), terrenos(terrenos) {
    this->tipo = tipo;
    // 16x16 para que sea vea mas grande
    this->setFixedSize(16, 16);
    QRect rect(x, y, 8, 8);
    // copio de la imagen completa de terrenos un cuadrado de 8x8
    QPixmap cropped = terrenos.copy(rect);
    // escalo la imagen a 16x16 para coincidir con tamaño del label
    this->setPixmap(cropped.scaled(16,16));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

int Label::get_posicion_x() {
    return this->x;
}

int Label::get_posicion_y() {
    return this->y;
}

string Label::get_tipo() {
    return this->tipo;
}

QPixmap Label::get_imagen() {
    QRect rect(this->x, this->y, 8, 8);
    QPixmap cropped (this->terrenos.copy(rect));
    return cropped.scaled(16, 16);
}

void Label::agregar_observador(Observador* observador_) {
    this->observador = observador_;
}

void Label::mousePressEvent(QMouseEvent* event) {
    emit clickeado();
}

void Label::clickeado() {
    this->observador->en_notificacion(this->id);
}

void Label::set_marco_clickeado() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}

void Label::borrar_marco_clickeado() {
    this->setLineWidth(0);
}

Label::~Label() {}
