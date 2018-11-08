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

void Mapa::en_notificacion(std::string id_label) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_imagen("/home/santiago/Documentos/TP Final Taller - Dune/editor/sprites/tablero_clickeado.png");
    }
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
            LabelMapa* label_mapa = new LabelMapa("/home/santiago/Documentos/TP Final Taller - Dune/editor/sprites/tablero.png", 
                id_label, this->parent);
            label_mapa->agregar_observador(this);
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            this->mapa.emplace(id_label, label_mapa);
        }
    }
}