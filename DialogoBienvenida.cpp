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

DialogoBienvenida::DialogoBienvenida(QWidget *parent) : QDialog(parent) {
    this->editor_fue_creado = false;
}

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

    QDialogButtonBox box_botones (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        this->editor = new Editor (line_edit_filas.text().toInt(), 
            line_edit_columnas.text().toInt(), 
            line_edit_cant_jugadores.text().toInt());
        this->editor_fue_creado = true;
        this->editor->show();

        this->close();
    }
}

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
    this->editor->show();

    this->close();
}

DialogoBienvenida::~DialogoBienvenida() {
    if (this->editor_fue_creado) {
        delete this->editor;
    }
}
