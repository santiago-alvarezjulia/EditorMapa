#include <iostream>
#include <QApplication>
#include <QFormLayout>
#include <QPushButton>
#include "Editor.h"
#include "DialogoBienvenida.h"

int main(int argc, char *argv[]) {
    // Clase que contiene el loop principal
    QApplication app(argc, argv);
    
    // instancio Dialogo Bienvenida
    DialogoBienvenida dialogo;

    QFormLayout form_layout (&dialogo);

    QLabel titulo ("Bienvenido al editor de mapas de Dune");
    form_layout.addRow(&titulo);

    QPushButton boton_nuevo_mapa ("CREAR MAPA");
    form_layout.addRow(&boton_nuevo_mapa);
    QObject::connect(&boton_nuevo_mapa, &QPushButton::clicked,
        &dialogo, &DialogoBienvenida::crear_mapa);

    QPushButton boton_cargar_mapa ("CARGAR MAPA");
    form_layout.addRow(&boton_cargar_mapa);
    QObject::connect(&boton_cargar_mapa, &QPushButton::clicked,
        &dialogo, &DialogoBienvenida::cargar_mapa);

    dialogo.show();

    // Arranca el loop de la UI
    return app.exec();
}
