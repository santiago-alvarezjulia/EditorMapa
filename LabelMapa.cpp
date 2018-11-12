#include "LabelMapa.h"
#include <string>
#include <iostream>
using std::string;

LabelMapa::LabelMapa(const QString& text, string id, QWidget* parent) : QLabel(parent),
    id(id) {
    this->setFixedSize(16, 16);
    this->setPixmap(QPixmap(text));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void LabelMapa::actualizar_imagen(QPixmap& nueva_imagen) {
    this->setPixmap(nueva_imagen.scaled(16, 16));
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
