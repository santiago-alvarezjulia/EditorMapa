#include "Editor.h"
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
#include <QSpinBox>
#include <QFileDialog>
#define MINIMO_CANTIDAD_JUGADORES 2
using std::string;

Editor::Editor(int filas, int columnas, int cant_jugadores, QWidget *parent) : 
    mapa(Mapa(filas, columnas, this)), tabs(Tabs(this)), cant_jugadores(cant_jugadores), 
    QWidget(parent, Qt::Window) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para que use esa configuracion. A partir de aca
    // puedo usar findChild
    editor.setupUi(this);

    // aca inicializo Tabs y Mapa para poder utilizar findChild
    this->tabs.inicializar_tabs();
    this->mapa.inicializar_mapa();

    // agrego al editor como observador del mapa
    this->mapa.agregar_observador(this);

    inicializar_ui();
    set_minimo_spin_box_jugadores(MINIMO_CANTIDAD_JUGADORES);
}


Editor::Editor(string& filename_json, QWidget *parent) : QWidget(parent, 
    Qt::Window), mapa(Mapa(this)), tabs(Tabs(this)) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion. A partir de aca
    // puedo usar findChild
    editor.setupUi(this);

    // aca inicializo Tabs y Mapa para poder utilizar findChild
    this->tabs.inicializar_tabs();
    this->mapa.parsear_json(filename_json);
    this->cant_jugadores = this->mapa.get_cantidad_jugadores_agregados();

    // agrego al editor como observador del mapa
    this->mapa.agregar_observador(this);

    inicializar_ui();
    set_minimo_spin_box_jugadores(this->cant_jugadores);
}


void Editor::en_notificacion(string& id_label_mapa) {
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
        }
    }

    // actualizo el valor minimo de spin_box_cantidad_jugadores
    int cantidad_jugadores_agregados = 
        this->mapa.get_cantidad_jugadores_agregados();
    if (cantidad_jugadores_agregados < MINIMO_CANTIDAD_JUGADORES) {
        set_minimo_spin_box_jugadores(MINIMO_CANTIDAD_JUGADORES);
    } else {
        set_minimo_spin_box_jugadores(cantidad_jugadores_agregados);
    }
}


void Editor::inicializar_ui() {
    // Conecto el evento del boton guardar mapa
    QPushButton* boton_guardar_mapa = findChild<QPushButton*>("botonGuardarMapa");
    QObject::connect(boton_guardar_mapa, &QPushButton::clicked, this, 
        &Editor::guardar_mapa);

    // Conecto el evento del boton cambiar tamanio del mapa
    QPushButton* boton_cambiar_tamanio_mapa = 
        findChild<QPushButton*>("botonCambiarTamanioMapa");
    QObject::connect(boton_cambiar_tamanio_mapa, &QPushButton::clicked, this, 
        &Editor::cambiar_tamanio_mapa);

    // Conecto el evento del boton cambiar cantidad de jugadores
    QPushButton* boton_cambiar_cantidad_jugadores = 
        findChild<QPushButton*>("botonCambiarCantidadJugadores");
    QObject::connect(boton_cambiar_cantidad_jugadores, &QPushButton::clicked, this, 
        &Editor::cambiar_cantidad_jugadores);

    // setteo el spinbox de cantidad de jugadores
    this->spin_box_cantidad_jugadores = 
        findChild<QSpinBox*>("spinBoxCantidadJugadores");
}

void Editor::set_minimo_spin_box_jugadores(int valor_minimo) {
    this->spin_box_cantidad_jugadores->setMinimum(valor_minimo);
}

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
    string nombre_archivo_std_string = nombre_archivo.toStdString();
    this->mapa.generar_json(nombre_archivo_std_string);

    // muestro mensaje de que se guardo el mapa correctamente
    QMessageBox::information(this, "Mapa guardado", 
            "Se guardo el mapa correctamente.");
}


void Editor::cambiar_tamanio_mapa() {
    QSpinBox* spin_box_filas = findChild<QSpinBox*>("spinBoxFilas");
    QSpinBox* spin_box_columnas = findChild<QSpinBox*>("spinBoxColumnas");
    
    this->mapa.cambiar_tamanio(spin_box_filas->value(), 
        spin_box_columnas->value());

    // actualizo el valor minimo de spin_box_cantidad_jugadores
    int cantidad_jugadores_agregados = 
        this->mapa.get_cantidad_jugadores_agregados();
    if (cantidad_jugadores_agregados < MINIMO_CANTIDAD_JUGADORES) {
        set_minimo_spin_box_jugadores(MINIMO_CANTIDAD_JUGADORES);
    } else {
        set_minimo_spin_box_jugadores(cantidad_jugadores_agregados);
    }

    // muestro mensaje de que se cambio el tamaño del mapa correctamente
    QMessageBox::information(this, "Tamaño del Mapa", 
            "Tamaño del mapa cambiado correctamente.");
}


void Editor::cambiar_cantidad_jugadores() {
    this->cant_jugadores = this->spin_box_cantidad_jugadores->value();
    
    // muestro mensaje de que se cambio la cantidad de jugadores correctamente
    QMessageBox::information(this, "Cantidad de jugadores", 
            "Cantidad de jugadores cambiados correctamente.");
}
