#include "LabelMapa.h"
#include <string>
#include <iostream>
using std::string;

LabelMapa::LabelMapa(const QString& text, string id, int pos_x, int pos_y, 
    string tipo, QWidget* parent) : QLabel(parent), id(id), tipo(tipo), 
    posicion_x(pos_x), posicion_y(pos_y) {
    this->setFixedSize(16, 16);
    this->setPixmap(QPixmap(text));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void LabelMapa::actualizar_data(QPixmap& nueva_imagen, int nueva_posicion_x,
            int nueva_posicion_y, std::string nuevo_tipo) {
    this->setPixmap(nueva_imagen.scaled(16, 16));
    this->posicion_x = nueva_posicion_x;
    this->posicion_y = nueva_posicion_y;
    this->tipo = nuevo_tipo; 
}

void LabelMapa::agregar_observador(ObservadorMapa* observador_) {
    this->observador = observador_;
}

void LabelMapa::mousePressEvent(QMouseEvent* event) {
    emit clickeado();
}

void LabelMapa::enterEvent(QEvent* event) {
    this->observador->label_mapa_enter_event(this->id);
}

void LabelMapa::leaveEvent(QEvent* event) {
    this->observador->label_mapa_leave_event(this->id);
}

void LabelMapa::set_marco_mouse_enter() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}

void LabelMapa::borrar_marco_mouse_enter() {
    this->setLineWidth(0);
}

void LabelMapa::clickeado() {
    this->observador->label_mapa_clickeado(this->id);
}

LabelMapa::~LabelMapa() {}
