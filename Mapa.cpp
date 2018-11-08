#include "Mapa.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "LabelMapa.h"
using std::string;
using std::map;

Mapa::Mapa(QWidget* parent) : parent(parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    inicializar_mapa();
}

void Mapa::agregar_observador(Observador* observer) {
    this->observador = observer;
}

void Mapa::actualizar_imagen(string id_label) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_imagen("/home/santiago/Documentos/editor mapa/EditorMapa/sprites/tablero_clickeado.png");
    }
}

void Mapa::en_notificacion(string id_label) {
    this->observador->en_notificacion(id_label);
}

void Mapa::inicializar_mapa() {
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    int dimensions = 20;
    for (int i = 0; i < dimensions; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            string id_label ("mapa");
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            LabelMapa* label_mapa = new LabelMapa("/home/santiago/Documentos/editor mapa/EditorMapa/sprites/tablero.png", 
                id_label, this->parent);
            label_mapa->agregar_observador(this);
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            this->mapa.emplace(id_label, label_mapa);
        }
    }
}