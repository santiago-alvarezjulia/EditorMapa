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
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& data_general = *it;
    
    ++it;
    const json& elem = *it;

    auto it_tiles = elem["sprites"].begin();
    json tile = *it;
    string id = tile["sprites"][0]["id"];
    string tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];

    for (int i = 0; i < this->filas; ++i) {
        for (int j = 0; j < this->columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(this->imagen_terrenos, id, tipo,
                pos_tiles, pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    map_layout->setSpacing(0);

    scroll_area_mapa->setLayout(map_layout);
}

bool Mapa::es_valido() {
    // IMPLEMENTAR -> no es valido unicamente con lo de los jugadores
    return true;
}

void Mapa::generar_json(std::string nombre_archivo) {
    json j;
    
    vector<vector<string>> tipos;
    vector<vector<uint32_t>> sprites;
    int largo_vector_sprites = this->columnas * 4;
    int cant_vectores_sprites = this->filas * 4;
    int cont_sprites_agregados = 0;

    for (int i = 0; i < this->filas; ++i) {
        vector<string> tipos_por_columna;
        vector<uint32_t> sprites_por_columna;
        
        for (int j = 0; j < this->columnas; ++j) {
            string id_label ("");
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	        if (it != this->mapa.end()) {
                tipos_por_columna.emplace_back(it->second->get_tipo());
                
                vector<uint32_t> pos_tiles_label = it->second->get_pos_tiles();
                vector<uint32_t>::iterator it_pos_tiles_label = pos_tiles_label.begin();
                for (; it_pos_tiles_label != pos_tiles_label.end(); ++it_pos_tiles_label) {
                    sprites_por_columna.emplace_back(*it_pos_tiles_label);
                    cont_sprites_agregados++;
                    if (cont_sprites_agregados == largo_vector_sprites) {
                        sprites.emplace_back(sprites_por_columna);
                        sprites_por_columna.clear();
                        cont_sprites_agregados = 0;
                    }
                }
            }
        }

        tipos.emplace_back(tipos_por_columna);
    }

    j["sprite"] = sprites;
    j["tipo"] = tipos;

    // genero el archivo mapa.json
    std::ofstream file(nombre_archivo);
    file << j;
}

void Mapa::agregar_observador(Observador* observer) {
    this->observador = observer;
}

void Mapa::actualizar_data(string id_label, QPixmap& nueva_imagen, 
    vector<uint32_t> nuevas_pos_tiles, string nuevo_tipo) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_data(nueva_imagen, nuevas_pos_tiles, nuevo_tipo);
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
            string id_label ("");
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
        