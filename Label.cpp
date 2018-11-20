#include "Label.h"
#include <string>
#include <iostream>
#include <QPainter>
using std::string;
using std::vector;

/**
 * \brief Constructor de Label.
 * 
 * Constructor de Label que recibe como parametro el QPixmap correspondiente
 * a la imagen .bmp que contiene todos los sprites de los terrenos. Tambien
 * el id, el tipo y la posicion de los tiles de este Label en particular.
 */
Label::Label(QPixmap& terrenos, string id, string tipo, vector<uint32_t> pos_tiles, 
    QWidget* parent) : QLabel(parent), id(id), tipo(tipo), pos_tiles(pos_tiles) {
    // fijo el tamaño de Label a 32x32 pixeles. 
    this->setFixedSize(32, 32);
    QPixmap label_32_x_32 (32, 32);

    // junto los 16 tiles de 8x8 pixeles, cuyas posiciones se encuentran en 
    // pos_tiles y los dibujo en un unico QPixmap de 32x32 pixeles. 
    int pos_x_label = 0;
    int pos_y_label = 0;
    vector<uint32_t>::iterator it_pos_tiles = pos_tiles.begin();
    for (int cont_tiles = 0; it_pos_tiles != pos_tiles.end(); ++it_pos_tiles) {
        // 8 es el tamaño individual de cada sprite (8x8 pixeles).
        // 20 es el ancho del archivo .bmp que contiene todos los sprites 
        // de los terrenos.
        // x e y son las posiciones de cada sprite de 8x8 en el archivo .bmp .
        int x;
        if (*it_pos_tiles < 20) {
            x = (*it_pos_tiles) * 8;
        } else {
            x = (*it_pos_tiles % 20) * 8;
        }
        int y = (*it_pos_tiles / 20) * 8;

        // copio el cuadrado de 8x8 que quiero del .bmp .
        QRect rect(x, y, 8, 8);
        QPixmap cropped = terrenos.copy(rect);
    
        // pos_x_label y pos_y_label es la posicion de cada sprite de 8x8 dentro
        // del QPixmap de 32x32.
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
 * \brief Getter posicion de tiles del Label.
 * 
 * Devuelvo un vector con las posiciones de los tiles dentro del archivo de 
 * terrenos.
 */
vector<uint32_t> Label::get_pos_tiles() {
    return this->pos_tiles;
}

/**
 * \brief Getter tipo del Label.
 * 
 * Devuelvo el tipo del Label.
 */
string Label::get_tipo() {
    return this->tipo;
}

/**
 * \brief Getter imagen del Label.
 * 
 * Devuelvo la imagen del Label.
 */
QPixmap Label::get_imagen() {
    return this->pixmap;
}

/**
 * \brief Agrego observador del Label.
 * 
 * Agrego un observador del Label (Tabs).
 */
void Label::agregar_observador(Observador* observador_) {
    this->observador = observador_;
}

/**
 * \brief Atrapo mouse clicks sobre el Label.
 * 
 * Emito la señal clickeado 
 */
void Label::mousePressEvent(QMouseEvent* event) {
    emit clickeado();
}

/**
 * \brief Señal clickeado del Label.
 * 
 * Le aviso al observador que fue clickeado.
 */
void Label::clickeado() {
    this->observador->en_notificacion(this->id);
}

/**
 * \brief Agrego el marco clickeado.
 * 
 * Agrego el marco clickeado.
 */
void Label::set_marco_clickeado() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}

/**
 * \brief Borro el marco clickeado.
 * 
 * Borro el marco clickeado.
 */
void Label::borrar_marco_clickeado() {
    this->setLineWidth(0);
}

/**
 * \brief Destructor de Label.
 * 
 * Destructor de Label.
 */
Label::~Label() {}
