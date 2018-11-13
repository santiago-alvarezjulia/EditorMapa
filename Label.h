#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "Observador.h"

class Label : public QLabel {
    public:
	    Label(QPixmap& terrenos, std::string id, int posicion_x, int posicion_y, 
            QWidget* parent = 0);
        int get_posicion_x();
        int get_posicion_y();
        std::string get_tipo();
        QPixmap get_imagen();
	    void set_marco_clickeado();
        void borrar_marco_clickeado();
        void agregar_observador(Observador* observador);
        ~Label();
 
    signals:
	    void clickeado();
 
    protected:
	    void mousePressEvent(QMouseEvent * event);

    private:
        std::string id;
        std::string tipo;
        int x;
        int y;
        QPixmap& terrenos;
        Observador* observador;
};

#endif // LABEL_H
