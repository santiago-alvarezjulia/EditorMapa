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
    this->mapa = map<string, LabelMapa*>();
    this->jugadores = map<string, bool>();
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
}

Mapa::Mapa(QWidget* parent) : parent(parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    this->jugadores = map<string, bool>();
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
}

void Mapa::parsear_json(string filename_json) {
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos
    std::ifstream entrada(filename_json);
    json mapa_json;
    entrada >> mapa_json;

    vector<vector<uint32_t>> pos_jugadores = mapa_json["jugadores"];
    vector<vector<string>> tipos = mapa_json["tipo"];
    int cant_fila = tipos.size();
    this->filas = cant_fila;
    vector<vector<uint32_t>> sprites = mapa_json["sprite"];

    vector<vector<uint32_t>> sprites_reacomodados = 
        reacomodar_sprites_json(sprites);
    
    vector<vector<uint32_t>>::iterator it_sprites_reacomodados = 
        sprites_reacomodados.begin();
    vector<vector<string>>::iterator it_filas_tipos = tipos.begin();
    int cant_columnas = (*it_filas_tipos).size();
    this->columnas = cant_columnas;

    int fila_actual = 0;
    int columna_actual = 0;
    for (; it_filas_tipos != tipos.end(); ++it_filas_tipos) {
        vector<string>::iterator it_tipos = (*it_filas_tipos).begin();
        for (; it_tipos != (*it_filas_tipos).end(); ++it_tipos) {
            string tipo = *it_tipos;
            
            string pos_label ("");
            pos_label += std::to_string(fila_actual);
            pos_label += DELIM_ID;
            pos_label += std::to_string(columna_actual);
            
            LabelMapa* label_mapa = new LabelMapa(this->imagen_terrenos, tipo,
                (*it_sprites_reacomodados), pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, fila_actual + 1, columna_actual + 1);
            
            this->mapa.emplace(pos_label, label_mapa);
            
            columna_actual++;
            it_sprites_reacomodados++;
            
            if (columna_actual == cant_columnas) {
                columna_actual = 0;
                fila_actual++;
            } 

        }
    }

    vector<vector<uint32_t>>::iterator it_pos_jugadores = pos_jugadores.begin();
    for (; it_pos_jugadores != pos_jugadores.end(); ++it_pos_jugadores) {
        string id_label ("");
        id_label += std::to_string((*it_pos_jugadores)[0]);
        id_label += ',';
        id_label += std::to_string((*it_pos_jugadores)[1]);
        this->jugadores.emplace(id_label, true); 
    }

    this->cantidad_jugadores = pos_jugadores.size();

    map_layout->setSpacing(0);
    scroll_area_mapa->setLayout(map_layout);
}


vector<vector<uint32_t>> Mapa::reacomodar_sprites_json(vector<vector<uint32_t>> sprites) {
    vector<vector<uint32_t>> sprites_reacomodado;
    vector<vector<uint32_t>>::iterator it_sprites = sprites.begin();
    vector<uint32_t> pos_tiles_32_x_32;
    int largo_pos_tiles_32_x_32 = 16;
    int pos_actual_tiles_32_x_32 = 0;

    for (; it_sprites != sprites.end(); ++it_sprites) {
        vector<uint32_t>::iterator it_sprites_vect = (*it_sprites).begin();
        for (; it_sprites_vect != (*it_sprites).end(); ++it_sprites_vect) {
            pos_tiles_32_x_32.emplace_back(*it_sprites_vect);
            pos_actual_tiles_32_x_32++;
            if (pos_actual_tiles_32_x_32 == largo_pos_tiles_32_x_32) {
                sprites_reacomodado.emplace_back(pos_tiles_32_x_32);
                pos_tiles_32_x_32.clear();
                pos_actual_tiles_32_x_32 = 0;
            }
        }
    }

    return sprites_reacomodado;
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
            
            LabelMapa* label_mapa = new LabelMapa(this->imagen_terrenos, tipo,
                pos_tiles, pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    map_layout->setSpacing(0);
    scroll_area_mapa->setLayout(map_layout);
}

int Mapa::get_cantidad_jugadores() {
    return this->cantidad_jugadores;
}

string Mapa::get_tipo_by_id(string id_label_mapa) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label_mapa);
	if (it != this->mapa.end()) {
        return it->second->get_tipo();
    }
}

bool Mapa::es_valido_agregar_jugador(string id_label_mapa) {
    if (this->jugadores.find(id_label_mapa) == this->jugadores.end()) {
        return true;
    }
    return false;
}

void Mapa::generar_json(std::string nombre_archivo) {
    json j;
    
    vector<vector<string>> tipos;
    vector<vector<uint32_t>> sprites;
    vector<vector<uint32_t>> jugadores_json;
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

    vector<uint32_t> pos_jugadores;
    map<string, bool>::iterator it_jugadores = this->jugadores.begin();
    for (; it_jugadores != this->jugadores.end(); ++it_jugadores) {
        vector<string> splitteado = this->split(it_jugadores->first, DELIM_ID);
        pos_jugadores.emplace_back(std::stoi(splitteado[0]));
        pos_jugadores.emplace_back(std::stoi(splitteado[1]));
        jugadores_json.emplace_back(pos_jugadores);
        pos_jugadores.clear();
    }

    // agrego data al json
    j["sprite"] = sprites;
    j["tipo"] = tipos;
    j["jugadores"] = jugadores_json;

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

void Mapa::agregar_jugador(string id_label, QPixmap& nueva_imagen) {
    this->jugadores.emplace(id_label, true);
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_imagen(nueva_imagen);
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

int Mapa::get_cantidad_jugadores_agregados() {
    return this->jugadores.size();
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
        