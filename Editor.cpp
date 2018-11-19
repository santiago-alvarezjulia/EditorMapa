#include "Editor.h"
// Cargo el archivo generado por uic, leer el CMakelist.txt para mas info
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
#include <vector>
#include <QFileDialog>
using std::string;
using std::vector;

Editor::Editor(int filas, int columnas, int cant_jugadores, QWidget *parent) : 
    cant_jugadores(cant_jugadores), QWidget(parent, Qt::Window), tabs(Tabs(this)),
    mapa(Mapa(filas, columnas, this)) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion
    editor.setupUi(this);

    this->tabs.inicializar_tabs();
    this->mapa.inicializar_mapa();
    this->mapa.agregar_observador(this);

    conectar_botones();
}

Editor::Editor(std::string filename_json, QWidget *parent) : QWidget(parent, 
    Qt::Window), tabs(Tabs(this)), mapa(Mapa(this)) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion
    editor.setupUi(this);

    this->tabs.inicializar_tabs();
    this->mapa.parsear_json(filename_json);
    this->mapa.agregar_observador(this);

    conectar_botones();
}

void Editor::en_notificacion(string id_label_mapa) {
    if (this->tabs.get_id_label_clickeado() != "") {
        QPixmap nueva_imagen = this->tabs.get_imagen_clickeado();
        vector<uint32_t> nuevas_pos_tiles = this->tabs.get_pos_tiles_clickeado();
        string nuevo_tipo = this->tabs.get_tipo_label_clickeado();
        this->mapa.actualizar_data(id_label_mapa, nueva_imagen, nuevas_pos_tiles, 
            nuevo_tipo);
        return;
    }
}

void Editor::conectar_botones() {
    // Conecto el evento del boton guardar mapa
    QPushButton* boton_guardar_mapa = findChild<QPushButton*>("botonGuardarMapa");
    QObject::connect(boton_guardar_mapa, &QPushButton::clicked, this, 
        &Editor::guardar_mapa);
}

void Editor::guardar_mapa() {
    bool es_mapa_valido = this->mapa.es_valido();
    if (!es_mapa_valido) {
        QMessageBox::critical(this, "Error al guardar mapa", 
            "Error con los jugadores");
        return;
    }

    QString nombre_archivo = QFileDialog::getSaveFileName(this, "Guardar mapa",
        QDir::currentPath(), "JSON (*.json)");
    if (nombre_archivo.isNull()) {
        return;
    }
    
    if(!nombre_archivo.contains(".json", Qt::CaseInsensitive)) {
        nombre_archivo += ".json";
    }
    this->mapa.generar_json(nombre_archivo.toStdString());
}

Editor::~Editor() {}
