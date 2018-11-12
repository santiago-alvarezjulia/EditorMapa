#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "Observador.h"

class Label : public QLabel {
    public:
	    Label(QPixmap& terrenos, std::string id, int ancho, int alto, int x,
            int y, QWidget * parent = 0);
        int get_ancho();
        int get_alto();
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
        int ancho;
        int alto;
        int x;
        int y;
        QPixmap& terrenos;
        Observador* observador;
};

#endif // LABEL_H
