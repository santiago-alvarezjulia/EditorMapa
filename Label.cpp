#include "Label.h"
#include <string>
#include <iostream>
using std::string;

Label::Label(QPixmap& terrenos, string id, int ancho, int alto, int x, int y,
    QWidget* parent) : QLabel(parent), id(id), ancho(ancho), alto(alto), x(x),
    y(y), terrenos(terrenos) {
    this->setFixedSize(16, 16);
    QRect rect(y, x, 8, 8);
    QPixmap cropped = terrenos.copy(rect);
    this->setPixmap(cropped.scaled(16,16));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

int Label::get_ancho() {
    return this->ancho;
}

int Label::get_alto() {
    return this->alto;
}

QPixmap Label::get_imagen() {
    QRect rect(this->y, this->x, 8, 8);
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
