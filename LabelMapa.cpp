#include "LabelMapa.h"
#include <string>
#include <vector>
#include <iostream>
#include <QPainter>
using std::string;
using std::vector;

LabelMapa::LabelMapa(QPixmap& terrenos, string id, string tipo, 
    vector<uint32_t> pos_tiles, string pos_label, QWidget* parent) : QLabel(parent), 
    id(id), tipo(tipo), pos_label(pos_label), terrenos(terrenos) {
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
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void LabelMapa::actualizar_data(QPixmap& nueva_imagen, 
    vector<uint32_t> nuevas_pos_tiles, string nuevo_tipo) {
    this->setPixmap(nueva_imagen);
    this->pos_tiles = nuevas_pos_tiles;
    this->tipo = nuevo_tipo; 
}

string LabelMapa::get_tipo() {
    return this->tipo;
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
