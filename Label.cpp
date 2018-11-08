#include "Label.h"
#include <string>
#include <iostream>
using std::string;

Label::Label(const QString & text, string id, QWidget* parent) : QLabel(parent),
    id(id) {
    this->setFixedSize(120, 86);
    this->setPixmap(QPixmap(text));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
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
