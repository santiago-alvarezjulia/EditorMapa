#include "Tabs.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "Label.h"
using std::string;
using std::map;

Tabs::Tabs(QWidget* parent) : parent(parent) {
    this->tab_terrenos = map<string, Label*>();
    this->id_label_clickeado = "";
    this->imagen_terrenos = QPixmap ("/home/santiago/Documentos/editor mapa/EditorMapa/sprites/terrain/d2k_BLOXBASE.bmp");
    agregar_terrenos();
}

string Tabs::get_id_label_clickeado() {
    return this->id_label_clickeado;
}

int Tabs::get_ancho_label_clickeado() {
    map<string, Label*>::iterator it = this->tab_terrenos.find(this->id_label_clickeado);
	if (it != this->tab_terrenos.end()) {
        return it->second->get_ancho();
    }
    return 0;
}

int Tabs::get_alto_label_clickeado() {
    map<string, Label*>::iterator it = this->tab_terrenos.find(this->id_label_clickeado);
	if (it != this->tab_terrenos.end()) {
        return it->second->get_alto();
    }
    return 0;
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
    QGridLayout* terrenos_layout = this->parent->findChild<QGridLayout*>("gridLayout_terrenos");
    QWidget* contenido_scroll_area = this->parent->findChild<QWidget*>("scrollArea_widget_contents_terrenos");
    int ancho_imagen_terrenos = this->imagen_terrenos.width() / 8;
    int alto_imagen_terrenos = this->imagen_terrenos.height() / 8;
    for (int i = 0; i < alto_imagen_terrenos; ++i) {
        for (int j = 0; j < ancho_imagen_terrenos; ++j) {
            string id_label ("terrenos");
            id_label += ',';
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            Label* label = new Label(this->imagen_terrenos, id_label, 1, 1, 
                i * 8, j * 8, this->parent);
            label->agregar_observador(this);
            terrenos_layout->addWidget(label, i + 1, j + 1);
            this->tab_terrenos.emplace(id_label, label);
        }
    }
    contenido_scroll_area->setLayout(terrenos_layout);
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
