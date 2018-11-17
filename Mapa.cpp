#include "Mapa.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "LabelMapa.h"
#include "libs/json.hpp"
#include <sstream>
#include <fstream>
#include <vector>
#define DELIM_ID ','
using std::string;
using std::stringstream;
using std::getline;
using std::vector;
using std::map;
using nlohmann::json;

Mapa::Mapa(int filas, int columnas, QWidget* parent) : filas(filas), 
    columnas(columnas), parent(parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
}

Mapa::Mapa(std::string filename_json, QWidget* parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    parsear_json(filename_json);
}

void Mapa::parsear_json(string filename_json) {

}

void Mapa::inicializar_mapa() {
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");

    // cosas de parseo del json de terrenos
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& valores_por_defecto = *it;
    ++it;

    json elem = valores_por_defecto;
    elem.update(*it);

    auto it_tiles = elem["pos_tiles"].begin();
    json tile = *it;
    string id = tile["pos_tiles"][0]["id"];
    string tipo = elem["tipo"];
    int x = tile["pos_tiles"][0]["x"];
    int y = tile["pos_tiles"][0]["y"];;

    for (int i = 0; i < this->filas; ++i) {
        for (int j = 0; j < this->columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(this->imagen_terrenos, id, 
                x, y, tipo, pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    map_layout->setSpacing(0);
}

bool Mapa::es_valido() {
    // IMPLEMENTAR
    return false;
}

void Mapa::generar_json() {
    // IMPLEMENTAR
}

void Mapa::agregar_observador(Observador* observer) {
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

void Mapa::label_mapa_clickeado(std::string id_label_mapa) {
    this->observador->en_notificacion(id_label_mapa);
}

void Mapa::label_mapa_enter_event(std::string id_label_mapa) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label_mapa);
	if (it != this->mapa.end()) {
        it->second->set_marco_mouse_enter();
    }
}

void Mapa::label_mapa_leave_event(std::string id_label_mapa) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label_mapa);
	if (it != this->mapa.end()) {
        it->second->borrar_marco_mouse_enter();
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

Mapa::~Mapa() {
    for (int i = 0; i < this->filas; ++i) {
        for (int j = 0; j < this->columnas; ++j) {
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
        