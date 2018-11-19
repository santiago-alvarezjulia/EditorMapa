#include "Label.h"
#include <string>
#include <iostream>
#include <QPainter>
using std::string;
using std::vector;

Label::Label(QPixmap& terrenos, string id, string tipo, vector<uint32_t> pos_tiles, 
    QWidget* parent) : QLabel(parent), id(id), tipo(tipo), pos_tiles(pos_tiles) {
    this->setFixedSize(32, 32);
    QPixmap label_32_x_32 (32, 32);

    int pos_x_label = 0;
    int pos_y_label = 0;
    vector<uint32_t>::iterator it_pos_tiles = pos_tiles.begin();
    for (int cont_tiles = 0; it_pos_tiles != pos_tiles.end(); ++it_pos_tiles) {
        int x;
        if (*it_pos_tiles < 20) {
            x = (*it_pos_tiles) * 8;
        } else {
            x = (*it_pos_tiles % 20) * 8;
        }

        int y = (*it_pos_tiles / 20) * 8;
        QRect rect(x, y, 8, 8);
        QPixmap cropped = terrenos.copy(rect);
    
        QPainter painter (&label_32_x_32);
        if (cont_tiles < 4) {
            pos_x_label = cont_tiles * 8;
        } else {
             pos_x_label = (cont_tiles % 4) * 8;
        }

        pos_y_label = (cont_tiles / 4) * 8;
        painter.drawPixmap(pos_x_label, pos_y_label, 8, 8, cropped);

        cont_tiles++;
    }
    
    this->setPixmap(label_32_x_32);
    this->pixmap = label_32_x_32;
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

vector<uint32_t> Label::get_pos_tiles() {
    return this->pos_tiles;
}

string Label::get_tipo() {
    return this->tipo;
}

QPixmap Label::get_imagen() {
    return this->pixmap;
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
