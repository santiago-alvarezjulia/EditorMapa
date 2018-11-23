#include "Editor.h"
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
#include <vector>
#include <QFileDialog>
using std::string;
using std::vector;

/**
 * \brief 1er Constructor de Editor.
 * 
 * Constructor de Editor que es utilizado cuando se crea un mapa. Recibe como 
 * parametros del mapa el tamaño (filas y columnas) y la cantidad de jugadores.
 */
Editor::Editor(int filas, int columnas, int cant_jugadores, QWidget *parent) : 
    cant_jugadores(cant_jugadores), QWidget(parent, Qt::Window) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para que use esa configuracion. A partir de aca
    // puedo usar findChild
    editor.setupUi(this);

    // aca inicializo Tabs y Mapa para poder utilizar findChild
    this->tabs = Tabs(this);
    this->tabs.inicializar_tabs();
    this->mapa = Mapa(filas, columnas, this);
    this->mapa.inicializar_mapa();

    // agrego al editor como observador del mapa
    this->mapa.agregar_observador(this);

    conectar_boton_guardar_mapa();
}

/**
 * \brief 2do Constructor de Editor.
 * 
 * Constructor de Editor que es utilizado cuando se carga un mapa. Recibe como
 * parametro el nombre del archivo del mapa. El archivo contiene la informacion
 * relacionada con el tamaño (filas y columnas) y la cantidad de jugadores.
 */
Editor::Editor(std::string filename_json, QWidget *parent) : QWidget(parent, 
    Qt::Window) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion. A partir de aca
    // puedo usar findChild
    editor.setupUi(this);

    // aca inicializo Tabs y Mapa para poder utilizar findChild
    this->tabs = Tabs(this);
    this->tabs.inicializar_tabs();
    this->mapa = Mapa(this);
    this->mapa.parsear_json(filename_json);
    this->cant_jugadores = this->mapa.get_cantidad_jugadores();

    // agrego al editor como observador del mapa
    this->mapa.agregar_observador(this);

    conectar_boton_guardar_mapa();
}

/**
 * \brief Metodo virtual de la interfaz Observador implementada por Editor.
 * 
 * Metodo virtual que es llamado por Mapa cuando un label del mapa (LabelMapa)
 * es clickeado. Recibe por parametro el id del LabelMapa.
 */
void Editor::en_notificacion(string id_label_mapa) {
    // getteo el id del Label clickeado en Tabs
    string tabs_id_label_clickeado = this->tabs.get_id_label_clickeado();

    // me fijo si hay algun Label clickeado en Tabs
    if (tabs_id_label_clickeado != "") {
        // getteo el tipo y la imagen del Label clickeado en Tabs
        int nuevo_tipo = this->tabs.get_tipo_label_clickeado();
        QPixmap nueva_imagen = this->tabs.get_imagen_clickeado();
        
        // me fijo si es el caso especial en que el tipo es Jugador (6)
        if (nuevo_tipo == 6) {
            // me fijo que el tipo de LabelMapa sea una Roca para poder apoyar
            // al jugador
            if (this->mapa.es_valido_agregar_jugador(id_label_mapa, this->cant_jugadores)) {
                // agrego al jugador al mapa
                this->mapa.agregar_jugador(id_label_mapa, nueva_imagen);
            } else {
                return;
            }
        } else {
            this->mapa.actualizar_data(id_label_mapa, nueva_imagen, nuevo_tipo,
                tabs_id_label_clickeado);
            return;
        }
    }
}

/**
 * \brief Conector de botones de la interfaz Editor.
 * 
 * Asocia el boton de guardar mapa de la interfaz Editor con la funcion 
 * Editor::guardar_mapa.
 */
void Editor::conectar_boton_guardar_mapa() {
    // Conecto el evento del boton guardar mapa
    QPushButton* boton_guardar_mapa = findChild<QPushButton*>("botonGuardarMapa");
    QObject::connect(boton_guardar_mapa, &QPushButton::clicked, this, 
        &Editor::guardar_mapa);
}

/**
 * \brief Funcion que guarda el mapa en un archivo.
 * 
 * Antes de guardar el mapa, verifica que sea valido (se fija en la cantidad
 * de jugadores ubicados en el mapa) y luego muestra un dialogo para elegir
 * el nombre del mapa y el directorio. Finalmente delega en Mapa la generacion
 * del archivo.
 */
void Editor::guardar_mapa() {
    int cantidad_jugadores_agregados = this->mapa.get_cantidad_jugadores_agregados();
    if (cantidad_jugadores_agregados != this->cant_jugadores) {
        QMessageBox::critical(this, "Error al guardar mapa", 
            "Falta agregar jugadores");
        return;
    }

    // muestra un dialogo para elegir el nombre del mapa y el directorio.
    QString nombre_archivo = QFileDialog::getSaveFileName(this, "Guardar mapa",
        QDir::currentPath(), "JSON (*.json)");
    if (nombre_archivo.isNull()) {
        return;
    }
    
    // me fijo si el usuario incluyo la extension del archivo al ingresar el
    // nombre del mismo.
    if(!nombre_archivo.contains(".json", Qt::CaseSensitive)) {
        nombre_archivo += ".json";
    }
    this->mapa.generar_json(nombre_archivo.toStdString());
}

/**
 * \brief Destructor de Editor.
 * 
 * Destructor de editor.
 */
Editor::~Editor() {}
