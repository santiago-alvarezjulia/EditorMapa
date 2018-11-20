#include "DialogoBienvenida.h"
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <iostream>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include "Editor.h"

/**
 * \brief Constructor de DialogoBienvenida.
 * 
 * Constructor de DialogoBienvenida que es utilizado como menu principal al 
 * iniciar el editor.
 */
DialogoBienvenida::DialogoBienvenida(QWidget *parent) : QDialog(parent) {
    this->editor_fue_creado = false;
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

    QLabel titulo ("Crear mapa");
    form_layout.addRow(&titulo);

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

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        this->editor = new Editor (line_edit_filas.text().toInt(), 
            line_edit_columnas.text().toInt(), 
            line_edit_cant_jugadores.text().toInt());
        this->editor_fue_creado = true;
        this->editor->show();

        // cierro DialogoBienvenida
        this->close();
    }
}

/**
 * \brief Muestro dialogo cargar mapa.
 * 
 * Es llamada al tocar el boton de cargar mapa (vease main.cpp). Muestra el 
 * dialogo para elegir al mapa previamente creado y almacenado en la pc.
 */
void DialogoBienvenida::cargar_mapa() {
    // filename incluye el filepath completo
    QString filename = QFileDialog::getOpenFileName(this, 
        "Seleccionar un mapa", "", "JSON files (*.json)");
    if (filename.isNull()) {
        // toco cancelar o no eligio ninguna foto
        return;
    }
    
    // cargar mapa
    this->editor = new Editor (filename.toStdString());
    this->editor_fue_creado = true;

    // cierro DialogoBienvenida
    this->close();
    
    this->editor->show();
}

/**
 * \brief Destructor DialogoBienvenida.
 * 
 * Verifica si fue creado el editor en el heap, en caso afirmativo, libera
 * los recursos.
 */
DialogoBienvenida::~DialogoBienvenida() {
    if (this->editor_fue_creado) {
        delete this->editor;
    }
}
