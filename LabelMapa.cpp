#include "LabelMapa.h"
#include <string>
#include <vector>
#include <iostream>
#include <QPainter>
using std::string;
using std::vector;

/**
 * \brief Constructor de LabelMapa.
 * 
 * Constructor de LabelMapa que recibe como parametro el QPixmap correspondiente. 
 * Tambien el pos_label, el tipo y la posicion de los tiles de este 
 * LabelMapa en particular.
 */
LabelMapa::LabelMapa(QPixmap& sprite, string id, int tipo, string pos_label, 
    QWidget* parent) : QLabel(parent), id(id), tipo(tipo), pos_label(pos_label), 
    terrenos(terrenos) {
    // fijo el tamaño de Label a 32x32 pixeles.
    this->setFixedSize(32, 32);
    this->setPixmap(sprite);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

/**
 * \brief Actualizar data del LabelMapa.
 * 
 * Actualizo la data del LabelMapa.
 */
void LabelMapa::actualizar_data(QPixmap& nueva_imagen, int nuevo_tipo, 
    string nuevo_id) {
    this->setPixmap(nueva_imagen);
    this->tipo = nuevo_tipo; 
    this->id = nuevo_id;
}

/**
 * \brief Actualizar imagen del LabelMapa.
 * 
 * Actualizo la imagen del LabelMapa.
 */
void LabelMapa::actualizar_imagen(QPixmap& nueva_imagen) {
    this->setPixmap(nueva_imagen);
}

/**
 * \brief Agrego imagen de jugador.
 * 
 * Agrego la imagen del jugador sobre el LabelMapa.
 */
void LabelMapa::agregar_imagen_jugador(QPixmap& sprite_jugador) {
    this->setPixmap(sprite_jugador);
}

/**
 * \brief Getter tipo del LabelMapa.
 * 
 * Devuelvo el tipo del LabelMapa.
 */
int LabelMapa::get_tipo() {
    return this->tipo;
} 


/**
 * \brief Getter id del LabelMapa.
 * 
 * Devuelvo el id del LabelMapa.
 */
string LabelMapa::get_id() {
    return this->id;
} 


/**
 * \brief Agrego observador del LabelMapa.
 * 
 * Agrego un observador del LabelMapa (Mapa).
 */
void LabelMapa::agregar_observador(ObservadorMapa* observador_) {
    this->observador = observador_;
}

/**
 * \brief Atrapo mouse clicks sobre el LabelMapa.
 * 
 * Emito la señal clickeado 
 */
void LabelMapa::mousePressEvent(QMouseEvent* event) {
    emit clickeado();
}

/**
 * \brief Atrapo entradas del mouse sobre el LabelMapa.
 * 
 * Le aviso al observador que el mouse entro al LabelMapa. 
 */
void LabelMapa::enterEvent(QEvent* event) {
    this->set_marco_mouse_enter();
}

/**
 * \brief Atrapo salidas del mouse del LabelMapa.
 * 
 * Le aviso al observador que el mouse salio del LabelMapa. 
 */
void LabelMapa::leaveEvent(QEvent* event) {
    this->borrar_marco_mouse_enter();
}

/**
 * \brief Agrego el marco clickeado.
 * 
 * Agrego el marco clickeado.
 */
void LabelMapa::set_marco_mouse_enter() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}

/**
 * \brief Borro el marco clickeado.
 * 
 * Borro el marco clickeado.
 */
void LabelMapa::borrar_marco_mouse_enter() {
    this->setLineWidth(0);
}

/**
 * \brief Señal clickeado del Label.
 * 
 * Le aviso al observador que fue clickeado.
 */
void LabelMapa::clickeado() {
    this->observador->label_mapa_clickeado(this->pos_label);
}

/**
 * \brief Destructor de LabelMapa.
 * 
 * Destructor de LabelMapa.
 */
LabelMapa::~LabelMapa() {}
