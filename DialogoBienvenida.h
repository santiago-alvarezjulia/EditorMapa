#ifndef DIALOGO_BIENVENIDA_H
#define DIALOGO_BIENVENIDA_H

#include <QDialog>

class DialogoBienvenida : public QDialog {
    public:
        explicit DialogoBienvenida(QWidget *parent = 0);
        void mostrar_dialog_dimension_mapa();
        void cargar_mapa();
        ~DialogoBienvenida(); 
};

#endif // DIALOGO_BIENVENIDA_H