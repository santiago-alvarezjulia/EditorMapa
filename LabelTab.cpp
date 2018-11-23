#include "LabelTab.h"
#include <string>
#include <iostream>
#include <QPainter>
using std::string;
using std::vector;

/**
 * \brief Constructor de LabelTab.
 * 
 * Constructor de LabelTab que recibe como parametro el QPixmap correspondiente
 * a la imagen .bmp que contiene todos los sprites de los terrenos. Tambien
 * el id, el tipo y la posicion de los tiles de este Label en particular.
 */
LabelTab::LabelTab(QPixmap& terrenos, string id, int tipo, vector<uint32_t> pos_tiles, 
    QWidget* parent) : QLabel(parent), id(id), tipo(tipo), pos_tiles(pos_tiles) {
    // fijo el tamaño de Label a 32x32 pixeles. 
    this->setFixedSize(32, 32);
    QPixmap label_32_x_32 (32, 32);

    // junto los 16 tiles de 8x8 pixeles, cuyas posiciones se encuentran en 
    // pos_tiles y los dibujo en un unico QPixmap de 32x32 pixeles. 
    // pos_x_label y pos_y_label es la posicion de cada sprite de 8x8 dentro
    // del QPixmap de 32x32.
    int pos_x_label = 0;
    int pos_y_label = 0;
    vector<uint32_t>::iterator it_pos_tiles = pos_tiles.begin();
    for (int cont_tiles = 0; it_pos_tiles != pos_tiles.end(); ++it_pos_tiles) {
        // 8 es el tamaño individual de cada sprite (8x8 pixeles).
        // 20 es el ancho del archivo .bmp que contiene todos los sprites 
        // de los terrenos.
        // x e y son las posiciones de cada sprite de 8x8 en el archivo .bmp .
        // los -1 es que arranco a contar desde 1.
        int y = ((*it_pos_tiles - 1) / 20) * 8;
        int x;
        if (*it_pos_tiles < 20) {
            x = (*it_pos_tiles - 1) * 8;
        } else {
            x = ((*it_pos_tiles - 1) % 20) * 8;
        }

        // copio el cuadrado de 8x8 que quiero del .bmp .
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

/**
 * \brief Getter posicion de tiles del LabelTab.
 * 
 * Devuelvo un vector con las posiciones de los tiles dentro del archivo de 
 * terrenos.
 */
vector<uint32_t> LabelTab::get_pos_tiles() {
    return this->pos_tiles;
}

/**
 * \brief Getter tipo del LabelTab.
 * 
 * Devuelvo el tipo del LabelTab.
 */
int LabelTab::get_tipo() {
    return this->tipo;
}

/**
 * \brief Getter imagen del LabelTab.
 * 
 * Devuelvo la imagen del LabelTab.
 */
QPixmap LabelTab::get_imagen() {
    return this->pixmap;
}

/**
 * \brief Agrego observador del LabelTab.
 * 
 * Agrego un observador del LabelTab (Tabs).
 */
void LabelTab::agregar_observador(Observador* observador_) {
    this->observador = observador_;
}

/**
 * \brief Atrapo mouse clicks sobre el LabelTab.
 * 
 * Emito la señal clickeado 
 */
void LabelTab::mousePressEvent(QMouseEvent* event) {
    emit clickeado();
}

/**
 * \brief Señal clickeado del LabelTab.
 * 
 * Le aviso al observador que fue clickeado.
 */
void LabelTab::clickeado() {
    this->observador->en_notificacion(this->id);
}

/**
 * \brief Agrego el marco clickeado.
 * 
 * Agrego el marco clickeado.
 */
void LabelTab::set_marco_clickeado() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}

/**
 * \brief Borro el marco clickeado.
 * 
 * Borro el marco clickeado.
 */
void LabelTab::borrar_marco_clickeado() {
    this->setLineWidth(0);
}

/**
 * \brief Destructor de Label.
 * 
 * Destructor de Label.
 */
LabelTab::~LabelTab() {}
