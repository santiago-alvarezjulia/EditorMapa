#include "Editor.h"
// Cargo el archivo generado por uic, leer el CMakelist.txt para mas info
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include "DialogoBienvenida.h"
using std::string;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion
    editor.setupUi(this);
    
    this->tabs = new Tabs(this);
}


void Editor::mostrar_dialog_dimension_mapa() {
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
        this->mapa = new Mapa(line_edit_filas.text().toInt(), 
            line_edit_columnas.text().toInt(), this);
        this->mapa->agregar_observador(this);

        conectar_botones();
    } else {
        //mostrar_dialog_bienvenida();
    }
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

void Editor::conectar_botones() {
    // Conecto el evento del boton guardar mapa
    QPushButton* boton_guardar_mapa = findChild<QPushButton*>("botonGuardarMapa");
    QObject::connect(boton_guardar_mapa, &QPushButton::clicked, this, 
        &Editor::guardar_mapa);
}

void Editor::guardar_mapa() {
    bool es_mapa_valido = this->mapa->es_valido();
    if (!es_mapa_valido) {
        QMessageBox::critical(this, "Error al guardar mapa", "Existen celdas vacías");
        return;
    }
    this->mapa->generar_json();
}

void Editor::cargar_mapa() {
    // filename incluye el filepath completo
    QString filename = QFileDialog::getOpenFileName(this, "Seleccionar un mapa", "", "JSON files (*.json)");
    if (filename.isNull()) {
        // toco cancelar
        return;
    }
    // cargar mapa
}

Editor::~Editor() {
    delete this->tabs;
    //delete this->mapa;
}