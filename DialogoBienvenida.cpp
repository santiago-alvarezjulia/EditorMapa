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

DialogoBienvenida::DialogoBienvenida(QWidget *parent) : QDialog(parent) {}

void DialogoBienvenida::mostrar_dialog_dimension_mapa() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    QLabel titulo ("Elegir dimension del mapa");
    form_layout.addRow(&titulo);

    QString descripcion_filas ("Filas");
    QLineEdit line_edit_filas (&dialog);
    form_layout.addRow(descripcion_filas, &line_edit_filas);

    QString descripcion_columnas ("Columnas");
    QLineEdit line_edit_columnas (&dialog);
    form_layout.addRow(descripcion_columnas, &line_edit_columnas);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&box_botones, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        std::cout << "aceptado" << std::endl;
        
        Editor* editor = new Editor (line_edit_filas.text().toInt(), 
            line_edit_columnas.text().toInt(), 0);
        editor->show();

        this->close();
    }
}

void DialogoBienvenida::cargar_mapa() {
    // filename incluye el filepath completo
    QString filename = QFileDialog::getOpenFileName(this, "Seleccionar un mapa", "", "JSON files (*.json)");
    if (filename.isNull()) {
        // toco cancelar
        return;
    }
    // cargar mapa
}

DialogoBienvenida::~DialogoBienvenida() {
    this->close();
}