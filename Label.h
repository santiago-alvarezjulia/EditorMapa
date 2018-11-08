#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "Observador.h"

class Label : public QLabel {
    public:
	    Label(const QString & text, std::string id, QWidget * parent = 0);
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
        Observador* observador;
};

#endif // LABEL_H
