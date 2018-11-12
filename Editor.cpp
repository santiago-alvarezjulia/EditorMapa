#include "Editor.h"
// Cargo el archivo generado por uic, leer el CMakelist.txt para mas info
#include "ui_Editor.h"
#include <iostream>
using std::string;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion
    editor.setupUi(this);
    
    this->tabs = new Tabs(this);
    this->mapa = new Mapa(this);
    this->mapa->agregar_observador(this);
}

void Editor::label_mapa_clickeado(string id_label_mapa) {
    // CAMBIAR
    if (this->tabs->get_id_label_clickeado() != "") {
        QPixmap nueva_imagen = this->tabs->get_imagen_clickeado();
        this->mapa->actualizar_imagen(id_label_mapa, nueva_imagen);
        return;
    }
}

void Editor::label_mapa_enter_event(std::string id_label_mapa){
    // CAMBIAR
    if (this->tabs->get_id_label_clickeado() != "") {
        int ancho = this->tabs->get_ancho_label_clickeado();
        int alto = this->tabs->get_alto_label_clickeado();
        this->mapa->set_marco_label_clickeado(id_label_mapa, ancho, alto);
    } else {
        this->mapa->set_marco_mouse_enter(id_label_mapa);
    }
}

void Editor::label_mapa_leave_event(std::string id_label_mapa) {
    // CAMBIAR
    if (this->tabs->get_id_label_clickeado() != "") {
        int ancho = this->tabs->get_ancho_label_clickeado();
        int alto = this->tabs->get_alto_label_clickeado();
        this->mapa->borrar_marco_label_clickeado(id_label_mapa, ancho, alto);
    } else {
        this->mapa->borrar_marco_mouse_enter(id_label_mapa);
    }
} 

Editor::~Editor() {
    delete this->tabs;
    delete this->mapa;
}