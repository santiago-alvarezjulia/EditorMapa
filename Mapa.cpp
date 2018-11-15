#include "Mapa.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "LabelMapa.h"
#include "libs/json.hpp"
#include <sstream>
#include <fstream>
#include <vector>
#define DIMENSION 40
#define DELIM_ID ','
using std::string;
using std::stringstream;
using std::getline;
using std::vector;
using std::map;
using nlohmann::json;

Mapa::Mapa(QWidget* parent) : parent(parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    inicializar_mapa();
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
            LabelMapa* label_mapa = new LabelMapa("../sprites/tablero.png", 
                id_label, -1, -1, "vacio", this->parent);
            label_mapa->agregar_observador(this);
            map_layout->addWidget(label_mapa, i, j);
            this->mapa.emplace(id_label, label_mapa);
        }
    }

    // junto los QLabel lo mas posible
    map_layout->setSpacing(0);
    map_layout->setContentsMargins(0, 0, 0, 0);
}

bool Mapa::es_valido() {
    // IMPLEMENTAR
    return false;
}

void Mapa::generar_json() {
    // IMPLEMENTAR
}

void Mapa::agregar_observador(ObservadorMapa* observer) {
    this->observador = observer;
}

void Mapa::actualizar_data(string id_label, QPixmap& nueva_imagen, 
    int nueva_posicion_x, int nueva_posicion_y, string nuevo_tipo) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_data(nueva_imagen, nueva_posicion_x, 
            nueva_posicion_y, nuevo_tipo);
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

vector<string> Mapa::split(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<string> elementos;
    while (std::getline(ss, item, delim)) {
        elementos.emplace_back(item);
    }
    return elementos;
}

Mapa::~Mapa() {
    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            string id_label ("mapa");
            id_label += ',';
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	        if (it != this->mapa.end()) {
                delete it->second;
            }
        }
    }
}
        