#include "Tabs.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "Label.h"
using std::string;
using std::map;

Tabs::Tabs(QWidget* parent) : parent(parent) {
    this->tab_terrenos = std::map<std::string, Label*>();
    this->tab_infanteria = std::map<std::string, Label*>();
    this->tab_vehiculos = std::map<std::string, Label*>();
    this->id_label_clikeado = "";
    agregar_terrenos();
}

void Tabs::en_notificacion(std::string id_label) {
    map<string, Label*>::iterator it = this->tab_terrenos.find(id_label);
	if (it != this->tab_terrenos.end()) {
        if (this->id_label_clikeado == id_label) {
            it->second->borrar_marco_clickeado();
        } else {
            it->second->set_marco_clickeado();
            map<string, Label*>::iterator it2 = this->tab_terrenos.find(this->id_label_clikeado);
	        if (it2 != this->tab_terrenos.end()) {
                it2->second->borrar_marco_clickeado();
            }
            this->id_label_clikeado = id_label;
        }
    }
}

void Tabs::agregar_terrenos() {
    QGridLayout* terrenos_layout = this->parent->findChild<QGridLayout*>("gridLayout_terrenos");
    QWidget* contenido_scroll_area = this->parent->findChild<QWidget*>("scrollArea_widget_contents_terrenos");
    int dimensions = 10;
    for (int i = 0; i < dimensions; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            string id_label ("terrenos");
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            Label* label = new Label("/home/santiago/Documentos/TP Final Taller - Dune/editor/sprites/images.png", 
                id_label, this->parent);
            label->agregar_observador(this);
            terrenos_layout->addWidget(label, i + 1, j + 1);
            this->tab_terrenos.emplace(id_label, label);
        }
    }
    contenido_scroll_area->setLayout(terrenos_layout);
}

void Tabs::agregar_infanteria() {

}

void Tabs::agregar_vehiculos() {

}