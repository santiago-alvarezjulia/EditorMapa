#include <iostream>
#include <QApplication>
#include <QFormLayout>
#include <QPushButton>
#include "Editor.h"
#include "DialogoBienvenida.h"

int main(int argc, char *argv[]) {
    // Clase que contiene el loop principal
    QApplication app(argc, argv);
    
    // instancio Dialogo Bienvenida y lo muestro.
    DialogoBienvenida dialogo;
    dialogo.show();

    // Arranca el loop de la UI
    return app.exec();
}
