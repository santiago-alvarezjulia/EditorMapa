#include "LabelMapa.h"
#include <iostream>
#define TAMANIO_LABEL_MAPA 32
using std::string;

LabelMapa::LabelMapa(QPixmap& sprite, string id, int tipo, string pos_label, 
    QWidget* parent) : QLabel(parent), id(id), tipo(tipo), pos_label(pos_label), 
    terrenos(terrenos) {
    this->setFixedSize(TAMANIO_LABEL_MAPA, TAMANIO_LABEL_MAPA);
    this->setPixmap(sprite);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}


void LabelMapa::actualizar_data(QPixmap& nueva_imagen, int nuevo_tipo, 
    string nuevo_id) {
    this->setPixmap(nueva_imagen);
    this->tipo = nuevo_tipo; 
    this->id = nuevo_id;
}


void LabelMapa::actualizar_imagen(QPixmap& nueva_imagen) {
    this->setPixmap(nueva_imagen);
}


int LabelMapa::get_tipo() {
    return this->tipo;
} 


string LabelMapa::get_id() {
    return this->id;
} 


void LabelMapa::agregar_observador(ObservadorMapa* observador_) {
    this->observador = observador_;
}


void LabelMapa::mousePressEvent(QMouseEvent* event) {
    this->observador->label_mapa_clickeado(this->pos_label);
}


void LabelMapa::enterEvent(QEvent* event) {
    this->set_marco_mouse_enter();
}


void LabelMapa::leaveEvent(QEvent* event) {
    this->borrar_marco_mouse_enter();
}


void LabelMapa::set_marco_mouse_enter() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}


void LabelMapa::borrar_marco_mouse_enter() {
    this->setLineWidth(0);
}
