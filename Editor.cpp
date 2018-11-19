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
    this->cant_jugadores = this->mapa.get_cantidad_jugadores();
    this->mapa.agregar_observador(this);

    conectar_botones();
}

void Editor::en_notificacion(string id_label_mapa) {
    string tabs_id_label_clickeado = this->tabs.get_id_label_clickeado();
    if (tabs_id_label_clickeado != "") {
        string nuevo_tipo = this->tabs.get_tipo_label_clickeado();
        QPixmap nueva_imagen = this->tabs.get_imagen_clickeado();
        if (nuevo_tipo == "jugador") {
            if (this->mapa.get_tipo_by_id(id_label_mapa) == "roca") {
                if (this->mapa.get_cantidad_jugadores_agregados() < this->cant_jugadores) {
                    if (this->mapa.es_valido_agregar_jugador(id_label_mapa)) {
                        this->mapa.agregar_jugador(id_label_mapa, nueva_imagen);
                    } else {
                        QMessageBox::critical(this, "Error al agregar un jugador", 
                            "Ya agregaste un jugador en esa posicion");
                        return;
                    }
                    
                } else {
                    QMessageBox::critical(this, "Error al agregar un jugador", 
                        "Ya alcanzaste el maximo de jugadores posibles");
                    return;
                }
                
            } else {
                QMessageBox::critical(this, "Error al agregar un jugador", 
                    "Solo se puede agregar jugadores sobre roca");
                return;
            }
        } else {
            vector<uint32_t> nuevas_pos_tiles = this->tabs.get_pos_tiles_clickeado();
            this->mapa.actualizar_data(id_label_mapa, nueva_imagen, nuevas_pos_tiles, 
                nuevo_tipo);
            return;
        }
    }
}

void Editor::conectar_botones() {
    // Conecto el evento del boton guardar mapa
    QPushButton* boton_guardar_mapa = findChild<QPushButton*>("botonGuardarMapa");
    QObject::connect(boton_guardar_mapa, &QPushButton::clicked, this, 
        &Editor::guardar_mapa);
}

void Editor::guardar_mapa() {
    int cantidad_jugadores_agregados = this->mapa.get_cantidad_jugadores_agregados();
    if (cantidad_jugadores_agregados != this->cant_jugadores) {
        QMessageBox::critical(this, "Error al guardar mapa", 
            "Falta agregar jugadores");
        return;
    }

    QString nombre_archivo = QFileDialog::getSaveFileName(this, "Guardar mapa",
        QDir::currentPath(), "JSON (*.json)");
    if (nombre_archivo.isNull()) {
        return;
    }
    
    if(!nombre_archivo.contains(".json", Qt::CaseSensitive)) {
        nombre_archivo += ".json";
    }
    this->mapa.generar_json(nombre_archivo.toStdString());
}

Editor::~Editor() {}
