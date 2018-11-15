#include "Tabs.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "Label.h"
#include "libs/json.hpp"
#include <fstream>
using std::string;
using std::map;
using nlohmann::json;

Tabs::Tabs(QWidget* parent) : parent(parent) {
    this->tab_terrenos = map<string, Label*>();
    this->id_label_clickeado = "";
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
    agregar_terrenos();
}

string Tabs::get_id_label_clickeado() {
    return this->id_label_clickeado;
}

int Tabs::get_posicion_x_label_clickeado() {
    map<string, Label*>::iterator it = this->tab_terrenos.find(this->id_label_clickeado);
	if (it != this->tab_terrenos.end()) {
        return it->second->get_posicion_x();
    }
    return 0;
}

int Tabs::get_posicion_y_label_clickeado() {
    map<string, Label*>::iterator it = this->tab_terrenos.find(this->id_label_clickeado);
	if (it != this->tab_terrenos.end()) {
        return it->second->get_posicion_y();
    }
    return 0;
}

string Tabs::get_tipo_label_clickeado() {
    map<string, Label*>::iterator it = this->tab_terrenos.find(this->id_label_clickeado);
	if (it != this->tab_terrenos.end()) {
        return it->second->get_tipo();
    }
    return "";
}

QPixmap Tabs::get_imagen_clickeado() {
    map<string, Label*>::iterator it = this->tab_terrenos.find(this->id_label_clickeado);
	if (it != this->tab_terrenos.end()) {
        return it->second->get_imagen();
    }
}

void Tabs::en_notificacion(std::string id_label) {
    map<string, Label*>::iterator it = this->tab_terrenos.find(id_label);
	if (it != this->tab_terrenos.end()) {
        if (this->id_label_clickeado == id_label) {
            it->second->borrar_marco_clickeado();
            this->id_label_clickeado = "";
        } else {
            it->second->set_marco_clickeado();
            map<string, Label*>::iterator it2 = this->tab_terrenos.find(this->id_label_clickeado);
	        if (it2 != this->tab_terrenos.end()) {
                it2->second->borrar_marco_clickeado();
            }
            this->id_label_clickeado = id_label;
        }
    }
}

void Tabs::agregar_terrenos() {
    // getteo el layout y el widget de Arena
    QGridLayout* arena_layout = this->parent->findChild<QGridLayout*>("gridLayout_arena");
    QWidget* scroll_area_arena = this->parent->findChild<QWidget*>("scrollArea_widget_arena");

    // getteo el layout y el widget de Cima
    QGridLayout* cima_layout = this->parent->findChild<QGridLayout*>("gridLayout_cima");
    QWidget* scroll_area_cima = this->parent->findChild<QWidget*>("scrollArea_widget_cima");
    
    std::ifstream entrada("../sprites/terrain/terrenos.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(int i = 0; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        Label* label = new Label(this->imagen_terrenos, elem["id"], elem["tipo"],
            elem["pos_tile"]["x"], elem["pos_tile"]["y"], this->parent);
        label->agregar_observador(this);
        if (elem["tipo"] == "cima") {
            cima_layout->addWidget(label, 0, i + 1);
        } else {
            arena_layout->addWidget(label, 0, i + 1);
        }
        
        this->tab_terrenos.emplace(elem["id"], label);
        i++;
    }

    scroll_area_arena->setLayout(arena_layout);
    scroll_area_cima->setLayout(cima_layout);
}

Tabs::~Tabs() {
    int ancho_imagen_terrenos = this->imagen_terrenos.width() / 8;
    int alto_imagen_terrenos = this->imagen_terrenos.height() / 8;
    for (int i = 0; i < alto_imagen_terrenos; ++i) {
        for (int j = 0; j < ancho_imagen_terrenos; ++j) {
            string id_label ("terrenos");
            id_label += ',';
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            map<string, Label*>::iterator it = this->tab_terrenos.find(id_label);
	        if (it != this->tab_terrenos.end()) {
                delete it->second;
            }
        }
    }
}
