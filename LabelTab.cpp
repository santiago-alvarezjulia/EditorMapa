#include "LabelTab.h"
#include <iostream>
#include <QPainter>
#define DIMENSION_LABEL 32
using std::string;
using std::vector;

LabelTab::LabelTab(string& id, int tipo, vector<uint32_t>& pos_tiles, 
    QWidget* parent) : QLabel(parent) {
    // fijo el tamaño de Label por DIMENSION_LABEL. 
    this->setFixedSize(DIMENSION_LABEL, DIMENSION_LABEL);
    
    // genero el sprite
    GeneradorSprites generador_sprites;
    this->sprite = generador_sprites.generar_sprite(id, tipo, pos_tiles);
    
    this->setPixmap(this->sprite.imagen);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}


int LabelTab::get_tipo() {
    return this->sprite.tipo;
}


string LabelTab::get_id() {
    return this->sprite.id;
}


QPixmap LabelTab::get_imagen() {
    return this->sprite.imagen;
}


void LabelTab::agregar_observador(Observador* observador_) {
    this->observador = observador_;
}


void LabelTab::mousePressEvent(QMouseEvent* event) {
    this->observador->en_notificacion(this->sprite.id);
}


void LabelTab::set_marco_clickeado() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}


void LabelTab::borrar_marco_clickeado() {
    this->setLineWidth(0);
}
