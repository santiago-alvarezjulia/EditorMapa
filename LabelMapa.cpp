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
 * Constructor de LabelMapa que recibe como parametro el QPixmap correspondiente
 * a la imagen .bmp que contiene todos los sprites de los terrenos. Tambien
 * el pos_label, el tipo y la posicion de los tiles de este LabelMapa en particular.
 */
LabelMapa::LabelMapa(QPixmap& terrenos, string tipo, vector<uint32_t> pos_tiles, 
    string pos_label, QWidget* parent) : QLabel(parent), tipo(tipo), 
    pos_label(pos_label), terrenos(terrenos), pos_tiles(pos_tiles) {
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
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

/**
 * \brief Actualizar data del LabelMapa.
 * 
 * Actualizo la data del LabelMapa.
 */
void LabelMapa::actualizar_data(QPixmap& nueva_imagen, 
    vector<uint32_t> nuevas_pos_tiles, string nuevo_tipo) {
    this->setPixmap(nueva_imagen);
    this->pos_tiles = nuevas_pos_tiles;
    this->tipo = nuevo_tipo; 
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
 * Agrego la imagen del jugador sobre el LabelMapa (hardcodeado). La lógica es
 * la misma a la utilizada en el constructor de LabelMapa.
 */
void LabelMapa::agregar_imagen_jugador() {
    // Hardcodeado pos_tiles de Jugador (terrenos.json)
    vector<uint32_t> pos_tiles = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
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
}

/**
 * \brief Getter tipo del LabelMapa.
 * 
 * Devuelvo el tipo del LabelMapa.
 */
string LabelMapa::get_tipo() {
    return this->tipo;
} 

/**
 * \brief Getter posicion de tiles del LabelMapa.
 * 
 * Devuelvo un vector con las posiciones de los tiles dentro del archivo de 
 * terrenos.
 */
vector<uint32_t> LabelMapa::get_pos_tiles() {
    return this->pos_tiles;
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
    this->observador->label_mapa_enter_event(this->pos_label);
}

/**
 * \brief Atrapo salidas del mouse del LabelMapa.
 * 
 * Le aviso al observador que el mouse salio del LabelMapa. 
 */
void LabelMapa::leaveEvent(QEvent* event) {
    this->observador->label_mapa_leave_event(this->pos_label);
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
