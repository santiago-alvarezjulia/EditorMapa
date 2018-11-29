#include "Editor.h"
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
#include <QSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#define MINIMO_CANTIDAD_JUGADORES 2
#define DIMENSION_MINIMA_MAPA 30
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
    this->min_cant_jugadores = MINIMO_CANTIDAD_JUGADORES;
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
    this->min_cant_jugadores = this->cant_jugadores;
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

    // actualizo el valor minimo de cantidad de jugadores
    int cantidad_jugadores_agregados = 
        this->mapa.get_cantidad_jugadores_agregados();
    if (cantidad_jugadores_agregados < MINIMO_CANTIDAD_JUGADORES) {
        this->min_cant_jugadores = MINIMO_CANTIDAD_JUGADORES;
    } else {
        this->min_cant_jugadores = cantidad_jugadores_agregados;
    }
}


void Editor::inicializar_ui() {
    // agrego QMenuBar
    this->menu_bar = new QMenuBar(this);
    QMenu* menu_archivo = this->menu_bar->addMenu("Archivo");
    QMenu* menu_editar = this->menu_bar->addMenu("Editar");
    menu_archivo->addAction("Guardar mapa", this, &Editor::guardar_mapa);
    menu_editar->addAction("Cambiar cantidad de jugadores", this, 
        &Editor::mostrar_dialogo_cantidad_jugadores);
    menu_editar->addAction("Cambiar tamaño mapa", this, 
        &Editor::mostrar_dialogo_tamanio_mapa);
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


void Editor::mostrar_dialogo_tamanio_mapa() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    QString descripcion_filas ("Filas");
    QSpinBox spinbox_filas (&dialog);
    spinbox_filas.setMinimum(DIMENSION_MINIMA_MAPA);
    form_layout.addRow(descripcion_filas, &spinbox_filas);

    QString descripcion_columnas ("Columnas");
    QSpinBox spinbox_columnas (&dialog);
    spinbox_columnas.setMinimum(DIMENSION_MINIMA_MAPA);
    form_layout.addRow(descripcion_columnas, &spinbox_columnas);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setWindowTitle("Cambiar tamaño mapa");

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        this->mapa.cambiar_tamanio(spinbox_filas.value(), 
            spinbox_columnas.value());

        // actualizo el valor minimo de cantidad de jugadores
        int cantidad_jugadores_agregados = 
            this->mapa.get_cantidad_jugadores_agregados();
        if (cantidad_jugadores_agregados < MINIMO_CANTIDAD_JUGADORES) {
            this->min_cant_jugadores = MINIMO_CANTIDAD_JUGADORES;
        } else {
            this->min_cant_jugadores = cantidad_jugadores_agregados;
        }
    }
}


void Editor::mostrar_dialogo_cantidad_jugadores() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    QString descripcion_cant_jugadores ("Cantidad de jugadores");
    QSpinBox spinbox_jugadores (&dialog);
    spinbox_jugadores.setMinimum(this->min_cant_jugadores);
    form_layout.addRow(descripcion_cant_jugadores, &spinbox_jugadores);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setWindowTitle("Cambiar cantidad de jugadores");

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        this->cant_jugadores = spinbox_jugadores.value();
    
        // muestro mensaje de que se cambio la cantidad de jugadores correctamente
        QMessageBox::information(this, "Cantidad de jugadores", 
            "Cantidad de jugadores cambiados correctamente.");
    }
}

Editor::~Editor () {
    delete this->menu_bar;
}