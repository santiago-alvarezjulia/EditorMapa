#include "Tabs.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "Label.h"
#include "libs/json.hpp"
#include <fstream>
#include <vector>
#define MAX_COLUMNA 4
using std::string;
using std::map;
using nlohmann::json;
using std::vector;

/**
 * \brief Constructor Tabs.
 * 
 * Es llamado por el constructor de Editor, por lo tanto se separa la creacion
 * de la inicializacion de Tabs (el Editor debe hacer setupUi para poder 
 * utilizar los elementos graficos de Qt como QGridLayout).
 */
Tabs::Tabs(QWidget* parent) : parent(parent) {
    this->tabs_terrenos = map<string, Label*>();
    this->id_label_clickeado = "";
    // hardcodeo el nombre del archivo .bmp con los sprites del terreno.
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
}

/**
 * \brief Inicializador Tabs.
 * 
 * Llena las pestañas o tabs de sprites. Los separa segun su tipo.
 */
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
    // sprited del archivo this->imagen_terrenos.
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
 
            Label* label = new Label(this->imagen_terrenos, tile["sprites"][i]["id"], 
                elem["tipo"], tile["sprites"][i]["pos_tiles"], this->parent);
                       
            label->agregar_observador(this);

            // me fijo de que tipo es y lo agrego al layout correspondiente.
            if (elem["tipo"] == "cima") {
                cima_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == "arena"){
                arena_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == "roca"){
                roca_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == "precipicio"){
                precipicio_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == "especia"){
                especia_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == "duna"){
                duna_layout->addWidget(label, fila, columna);
            } else if (elem["tipo"] == "jugador"){
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

/**
 * \brief Getter id label clickeado.
 * 
 * Devuelvo el id del label clickeado, puede estar vacio ("").
 */
string Tabs::get_id_label_clickeado() {
    return this->id_label_clickeado;
}

/**
 * \brief Getter posicion de tiles del label clickeado.
 * 
 * Devuelvo un vector con las posiciones de los tiles dentro del archivo 
 * this->imagen_terrenos del label clickeado (delego en Label). 
 * Precondicion -> tiene que haberse verificado que hay un label clickeado.
 */
vector<uint32_t> Tabs::get_pos_tiles_clickeado() {
    map<string, Label*>::iterator it = this->tabs_terrenos.find(this->id_label_clickeado);
	if (it != this->tabs_terrenos.end()) {
        return it->second->get_pos_tiles();
    }
}

/**
 * \brief Getter tipo del label clickeado.
 * 
 * Devuelvo el tipo del label clickeado (delego en Label). Precondicion -> tiene que haberse
 * verificado que hay un label clickeado.
 */
string Tabs::get_tipo_label_clickeado() {
    map<string, Label*>::iterator it = this->tabs_terrenos.find(this->id_label_clickeado);
	if (it != this->tabs_terrenos.end()) {
        return it->second->get_tipo();
    }
}

/**
 * \brief Getter imagen del label clickeado.
 * 
 * Devuelvo la imagen del label clickeado (delego en Label). 
 * Precondicion -> tiene que haberse verificado que hay un label clickeado.
 */
QPixmap Tabs::get_imagen_clickeado() {
    map<string, Label*>::iterator it = this->tabs_terrenos.find(this->id_label_clickeado);
	if (it != this->tabs_terrenos.end()) {
        return it->second->get_imagen();
    }
}

/**
 * \brief Metodo virtual de la interfaz Observador implementada por Tabs.
 * 
 * Metodo virtual que es llamado por Mapa cuando un label de una pestaña (Label)
 * es clickeado. Recibe por parametro el id del Label.
 */
void Tabs::en_notificacion(std::string id_label) {
    // me fijo que tenga almacenado al Label en tabs_terrenos
    map<string, Label*>::iterator it = this->tabs_terrenos.find(id_label);
	if (it != this->tabs_terrenos.end()) {
        // me fijo si el id coincide con el id del label clickeado actualmente.
        if (this->id_label_clickeado == id_label) {
            it->second->borrar_marco_clickeado();
            this->id_label_clickeado = "";
        } else {
            it->second->set_marco_clickeado();
            map<string, Label*>::iterator it2 = this->tabs_terrenos.find(this->id_label_clickeado);
	        if (it2 != this->tabs_terrenos.end()) {
                it2->second->borrar_marco_clickeado();
            }
            this->id_label_clickeado = id_label;
        }
    }
}

/**
 * \brief Destructor de Tabs.
 * 
 * Libero los Label del heap (fueron tomados los recursos en Tabs::inicializar_tabs)
 */
Tabs::~Tabs() {
    map<string, Label*>::iterator it = this->tabs_terrenos.begin();
    for (; it != this->tabs_terrenos.end(); ++it) {
        delete it->second;
    }
}
