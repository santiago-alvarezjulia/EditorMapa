#include "Tabs.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "LabelTab.h"
#include "libs/json.hpp"
#include <fstream>
#include <vector>
#define MAX_COLUMNA 4
using std::string;
using std::map;
using nlohmann::json;
using std::vector;

Tabs::Tabs(QWidget* parent) : parent(parent) {
    this->tabs_terrenos = map<string, LabelTab*>();
    Sprite sprite;
    sprite.id = "";
    this->sprite_clickeado = sprite;
}


void Tabs::inicializar_tabs() {
    // getteo el layout y el widget de Arena
    QGridLayout* arena_layout = this->parent->findChild<QGridLayout*>("gridLayout_arena");
    QWidget* scroll_area_arena = this->parent->findChild<QWidget*>("scrollArea_widget_arena");

    // getteo el layout y el widget de Cima
    QGridLayout* cima_layout = this->parent->findChild<QGridLayout*>("gridLayout_cima");
    QWidget* scroll_area_cima = this->parent->findChild<QWidget*>("scrollArea_widget_cima");
    
    // getteo el layout y el widget de Roca
    QGridLayout* roca_layout = this->parent->findChild<QGridLayout*>("gridLayout_roca");
    QWidget* scroll_area_roca = this->parent->findChild<QWidget*>("scrollArea_widget_roca");

    // getteo el layout y el widget de Precipio
    QGridLayout* precipicio_layout = this->parent->findChild<QGridLayout*>("gridLayout_precipicio");
    QWidget* scroll_area_precipicio = this->parent->findChild<QWidget*>("scrollArea_widget_precipicio");

    // getteo el layout y el widget de Duna
    QGridLayout* duna_layout = this->parent->findChild<QGridLayout*>("gridLayout_duna");
    QWidget* scroll_area_duna = this->parent->findChild<QWidget*>("scrollArea_widget_duna");

    // getteo el layout y el widget de Especia
    QGridLayout* especia_layout = this->parent->findChild<QGridLayout*>("gridLayout_especia");
    QWidget* scroll_area_especia = this->parent->findChild<QWidget*>("scrollArea_widget_especia");

    // getteo el layout y el widget de Jugador
    QGridLayout* jugador_layout = this->parent->findChild<QGridLayout*>("gridLayout_jugador");
    QWidget* scroll_area_jugador = this->parent->findChild<QWidget*>("scrollArea_widget_jugador");

    // hardcodeo la ubicacion del archivo .json con la informacion sobre los
    // sprites del archivo this->imagen_terrenos.
    std::ifstream entrada("../sprites/terrain/terrenos.json");

    json terrenos_json;

    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for (; it != terrenos_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        // valores donde ubicar los label dentro de los gridLayout de cada pestaña.
        int columna = 0;
        int fila = 0;

        auto it_sprites = elem["sprites"].begin();
        for (int i = 0; it_sprites != elem["sprites"].end(); ++it_sprites) {
            json tile = *it;

            string id = tile["sprites"][i]["id"];
            vector<uint32_t> pos_tiles = tile["sprites"][i]["pos_tiles"];
            
            LabelTab* label = new LabelTab(id, elem["tipo"], pos_tiles, 
                this->parent);
                       
            label->agregar_observador(this);

            // me fijo de que tipo es y lo agrego al layout correspondiente.
            if (elem["tipo"] == 5) {
                cima_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == 1){
                arena_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == 0){
                roca_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == 2){
                precipicio_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == 4){
                especia_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == 3){
                duna_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == 6){
                jugador_layout->addWidget(label, fila, columna);
            } 
        
            this->tabs_terrenos.emplace(tile["sprites"][i]["id"], label);
            
            i++;
            columna++;

            // fijo un maximo de columna para que el scrolleo sobre el 
            // QGridLayout sea solo vertical y no horizontal (cuestion estetica)
            if (columna == MAX_COLUMNA) {
                columna = 0;
                fila++;
            }
        }
    }

    // asocio los QGridLayout con sus respectivas scroll areas.
    scroll_area_arena->setLayout(arena_layout);
    scroll_area_cima->setLayout(cima_layout);
    scroll_area_roca->setLayout(roca_layout);
    scroll_area_precipicio->setLayout(precipicio_layout);
    scroll_area_especia->setLayout(especia_layout);
    scroll_area_duna->setLayout(duna_layout);
    scroll_area_jugador->setLayout(jugador_layout);
}

Sprite Tabs::get_sprite_clickeado() {
    return this->sprite_clickeado;
}


void Tabs::en_click_terreno_tab(Sprite sprite) {
    // me fijo que tenga almacenado al Label en tabs_terrenos
    map<string, LabelTab*>::iterator it = 
        this->tabs_terrenos.find(sprite.id);
	if (it != this->tabs_terrenos.end()) {
        // me fijo si el id coincide con el id del label clickeado actualmente.
        if (this->sprite_clickeado.id == sprite.id) {
            it->second->borrar_marco_clickeado();
            this->sprite_clickeado.id = "";
        } else {
            // no coinciedo => le agrego el marco de clickeado y borro el marco
            // del clickeado anteriormente.
            it->second->set_marco_clickeado();
            map<string, LabelTab*>::iterator it2 = 
                this->tabs_terrenos.find(this->sprite_clickeado.id);
	        if (it2 != this->tabs_terrenos.end()) {
                it2->second->borrar_marco_clickeado();
            }
            this->sprite_clickeado = sprite;
        }
    }
}

Tabs::~Tabs() {
    map<string, LabelTab*>::iterator it = this->tabs_terrenos.begin();
    for (; it != this->tabs_terrenos.end(); ++it) {
        delete it->second;
    }
}
