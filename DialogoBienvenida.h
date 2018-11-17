#ifndef DIALOGO_BIENVENIDA_H
#define DIALOGO_BIENVENIDA_H

#include <QDialog>
#include "Editor.h"

class DialogoBienvenida : public QDialog {
    public:
        explicit DialogoBienvenida(QWidget *parent = 0);
        void crear_mapa();
        void cargar_mapa();
        ~DialogoBienvenida(); 

    private:
        Editor* editor;
        bool editor_fue_creado;
};

#endif // DIALOGO_BIENVENIDA_H