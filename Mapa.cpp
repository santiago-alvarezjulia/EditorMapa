#include "Mapa.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "LabelMapa.h"
#include <sstream>
#include <vector>
#define DIMENSION 40
#define DELIM_ID ','
using std::string;
using std::stringstream;
using std::getline;
using std::vector;
using std::map;

Mapa::Mapa(QWidget* parent) : parent(parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    inicializar_mapa();
}

void Mapa::agregar_observador(ObservadorMapa* observer) {
    this->observador = observer;
}

void Mapa::actualizar_imagen(string id_label, QPixmap& nueva_imagen) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_imagen(nueva_imagen);
    }
}

void Mapa::set_marco_mouse_enter(string id_label) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->set_marco_mouse_enter();
    }
}

void Mapa::borrar_marco_mouse_enter(string id_label) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->borrar_marco_mouse_enter();
    }
}

void Mapa::label_mapa_clickeado(std::string id_label_mapa) {
    this->observador->label_mapa_clickeado(id_label_mapa);
}

void Mapa::label_mapa_enter_event(std::string id_label_mapa) {
    this->observador->label_mapa_enter_event(id_label_mapa);
}

void Mapa::label_mapa_leave_event(std::string id_label_mapa) {
    this->observador->label_mapa_leave_event(id_label_mapa);
}

void Mapa::inicializar_mapa() {
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            string id_label ("mapa");
            id_label += DELIM_ID;
            id_label += std::to_string(i);
            id_label += DELIM_ID;
            id_label += std::to_string(j);
            LabelMapa* label_mapa = new LabelMapa("/home/santiago/Documentos/editor mapa/EditorMapa/sprites/tablero.png", 
                id_label, this->parent);
            label_mapa->agregar_observador(this);
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            this->mapa.emplace(id_label, label_mapa);
        }
    }

    // junto los QLabel lo mas posible
    map_layout->setSpacing(0);
    map_layout->setContentsMargins(0, 0, 0, 0);
}

void Mapa::set_marco_label_clickeado(std::string id_label, int ancho, 
    int alto) {
    vector<string> data_id = split(id_label, DELIM_ID);
    int fila = std::stoi(data_id[1]);
    int columna = std::stoi(data_id[2]);

    for (int i = fila; i < (fila + ancho); ++i) {
        for (int j = columna; j < (columna + alto); ++j) {
            string label ("mapa");
            label += DELIM_ID;
            label += std::to_string(i);
            label += DELIM_ID;
            label += std::to_string(j);
            this->set_marco_mouse_enter(label);
        }
    }
}

void Mapa::borrar_marco_label_clickeado(std::string id_label, int ancho, 
    int alto) {
    vector<string> data_id = split(id_label, DELIM_ID);
    int fila = std::stoi(data_id[1]);
    int columna = std::stoi(data_id[2]);

    for (int i = fila; i < (fila + ancho); ++i) {
        for (int j = columna; j < (columna + alto); ++j) {
            string label ("mapa");
            label += DELIM_ID;
            label += std::to_string(i);
            label += DELIM_ID;
            label += std::to_string(j);
            this->borrar_marco_mouse_enter(label);
        }
    }
}


vector<string> Mapa::split(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<string> elementos;
    while (std::getline(ss, item, delim)) {
        elementos.emplace_back(item);
    }
    return elementos;
}
        