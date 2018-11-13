#include "Editor.h"
// Cargo el archivo generado por uic, leer el CMakelist.txt para mas info
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
using std::string;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion
    editor.setupUi(this);
    
    this->tabs = new Tabs(this);
    this->mapa = new Mapa(this);
    this->mapa->agregar_observador(this);

    conectar_boton_guardar();
}

void Editor::label_mapa_clickeado(string id_label_mapa) {
    if (this->tabs->get_id_label_clickeado() != "") {
        QPixmap nueva_imagen = this->tabs->get_imagen_clickeado();
        int nueva_posicion_x = this->tabs->get_posicion_x_label_clickeado();
        int nueva_posicion_y = this->tabs->get_posicion_y_label_clickeado();
        string nuevo_tipo = this->tabs->get_tipo_label_clickeado();
        this->mapa->actualizar_data(id_label_mapa, nueva_imagen, nueva_posicion_x,
            nueva_posicion_y, nuevo_tipo);
        return;
    }
}

void Editor::label_mapa_enter_event(std::string id_label_mapa){
    this->mapa->set_marco_mouse_enter(id_label_mapa);
}

void Editor::label_mapa_leave_event(std::string id_label_mapa) {
    this->mapa->borrar_marco_mouse_enter(id_label_mapa);
} 

void Editor::conectar_boton_guardar() {
    // Conecto el evento del boton
    QPushButton* boton_guardar_mapa = findChild<QPushButton*>("botonGuardarMapa");
    QObject::connect(boton_guardar_mapa, &QPushButton::clicked,
                     this, &Editor::guardar_mapa);
}

void Editor::guardar_mapa() {
    bool es_mapa_valido = this->mapa->es_valido();
    if (!es_mapa_valido) {
        QMessageBox::critical(this, "Error al guardar mapa", "Existen celdas vacías");
        return;
    }
    this->mapa->generar_json();
}

Editor::~Editor() {
    delete this->tabs;
    delete this->mapa;
}