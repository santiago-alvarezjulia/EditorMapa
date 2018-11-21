#ifndef DIALOGO_BIENVENIDA_H
#define DIALOGO_BIENVENIDA_H

#include <QDialog>
#include <QFormLayout>
#include "Editor.h"

class DialogoBienvenida : public QDialog {
    public:
        explicit DialogoBienvenida(QWidget *parent = 0);
        void crear_mapa();
        void cargar_mapa();
        ~DialogoBienvenida(); 

    private:
        Editor* editor;
        QFormLayout* form_layout;
        QLabel* titulo;
        QPushButton* boton_crear_mapa;
        QPushButton* boton_cargar_mapa;
        bool editor_fue_creado;
};

#endif // DIALOGO_BIENVENIDA_H