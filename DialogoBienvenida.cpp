#include "DialogoBienvenida.h"
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <iostream>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include "Editor.h"

/**
 * \brief Constructor de DialogoBienvenida.
 * 
 * Constructor de DialogoBienvenida que es utilizado como menu principal al 
 * iniciar el editor.
 */
DialogoBienvenida::DialogoBienvenida(QWidget *parent) : QDialog(parent) {
    this->editor_fue_creado = false;

    this->form_layout = new QFormLayout(this);

    this->titulo = new QLabel("Bienvenido al editor de mapas de Dune");
    form_layout->addRow(titulo);

    this->boton_crear_mapa = new QPushButton("CREAR MAPA");
    this->form_layout->addRow(this->boton_crear_mapa);
    QObject::connect(this->boton_crear_mapa, &QPushButton::clicked,
        this, &DialogoBienvenida::crear_mapa);

    this->boton_cargar_mapa = new QPushButton("CARGAR MAPA");
    this->form_layout->addRow(this->boton_cargar_mapa);
    QObject::connect(this->boton_cargar_mapa, &QPushButton::clicked,
        this, &DialogoBienvenida::cargar_mapa);

    this->setWindowTitle("Editor de Mapas de Dune");
}

/**
 * \brief Muestro dialogo crear mapa.
 * 
 * Es llamada al tocar el boton de crear mapa (vease main.cpp). Muestra el 
 * dialogo con las configuraciones posibles para un mapa nuevo 
 * (tamaño y cantidad de jugadores).
 */
void DialogoBienvenida::crear_mapa() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    QString descripcion_filas ("Filas");
    QLineEdit line_edit_filas (&dialog);
    form_layout.addRow(descripcion_filas, &line_edit_filas);

    QString descripcion_columnas ("Columnas");
    QLineEdit line_edit_columnas (&dialog);
    form_layout.addRow(descripcion_columnas, &line_edit_columnas);

    QString descripcion_cant_jugadores ("Cantidad de jugadores");
    QLineEdit line_edit_cant_jugadores (&dialog);
    form_layout.addRow(descripcion_cant_jugadores, &line_edit_cant_jugadores);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setWindowTitle("Crear mapa");

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        QString filas = line_edit_filas.text();
        QString columnas = line_edit_columnas.text();
        QString cantidad_de_jugadores = line_edit_cant_jugadores.text();
        
        if (filas.isEmpty() || columnas.isEmpty() || cantidad_de_jugadores.isEmpty()) {
            QMessageBox::critical(this, "Error al crear mapa", 
                "Te falto completar la configuación.");
        } else {
            this->editor = new Editor (filas.toInt(), columnas.toInt(), 
                cantidad_de_jugadores.toInt());
            this->editor_fue_creado = true;
            this->editor->show();

            // cierro DialogoBienvenida
            this->close();
        }
    }
}

/**
 * \brief Muestro dialogo cargar mapa.
 * 
 * Es llamada al tocar el boton de cargar mapa (vease main.cpp). Muestra el 
 * dialogo para elegir al mapa previamente creado y almacenado en la pc.
 */
void DialogoBienvenida::cargar_mapa() {
    // filename, incluye el filepath completo.
    QString filename = QFileDialog::getOpenFileName(this, 
        "Cargar mapa", "", "JSON files (*.json)");
    if (filename.isNull()) {
        // toco cancelar o no eligio ningun archivo json.
        return;
    }
    
    // cargar mapa
    this->editor = new Editor (filename.toStdString());
    this->editor_fue_creado = true;

    // cierro DialogoBienvenida y muestro el Editor.
    this->close();
    this->editor->show();
}

/**
 * \brief Destructor DialogoBienvenida.
 * 
 * Verifica si fue creado el editor en el heap, en caso afirmativo, lo libero.
 * Siempre liberp el titulo y los botones.
 */
DialogoBienvenida::~DialogoBienvenida() {
    if (this->editor_fue_creado) {
        delete this->editor;
    }

    delete this->form_layout;
    delete this->titulo;
    delete this->boton_crear_mapa;
    delete this->boton_cargar_mapa;
}
