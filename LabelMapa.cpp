#include "LabelMapa.h"
#include <string>
#include <iostream>
using std::string;

LabelMapa::LabelMapa(QPixmap& terrenos, string id, int pos_x, int pos_y, 
    string tipo, string pos_label, QWidget* parent) : QLabel(parent), id(id), 
    tipo(tipo), posicion_x(pos_x), posicion_y(pos_y), pos_label(pos_label), 
    terrenos(terrenos) {
    // 16x16 para que sea vea mas grande
    this->setFixedSize(16, 16);
    QRect rect(pos_x, pos_y, 8, 8);
    // copio de la imagen completa de terrenos un cuadrado de 8x8
    QPixmap cropped = terrenos.copy(rect);
    // escalo la imagen a 16x16 para coincidir con tamaño del label
    this->setPixmap(cropped.scaled(16,16));
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
    this->observador->label_mapa_enter_event(this->pos_label);
}

void LabelMapa::leaveEvent(QEvent* event) {
    this->observador->label_mapa_leave_event(this->pos_label);
}

void LabelMapa::set_marco_mouse_enter() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}

void LabelMapa::borrar_marco_mouse_enter() {
    this->setLineWidth(0);
}

void LabelMapa::clickeado() {
    this->observador->label_mapa_clickeado(this->pos_label);
}

LabelMapa::~LabelMapa() {}
