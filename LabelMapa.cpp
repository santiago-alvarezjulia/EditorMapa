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

void LabelMapa::actualizar_imagen(const QString& path_imagen) {
    this->setPixmap(QPixmap(path_imagen));
}

void LabelMapa::agregar_observador(Observador* observador_) {
    this->observador = observador_;
}

void LabelMapa::mousePressEvent(QMouseEvent* event) {
    emit clickeado();
}

void LabelMapa::clickeado() {
    this->observador->en_notificacion(this->id);
}

LabelMapa::~LabelMapa() {}
