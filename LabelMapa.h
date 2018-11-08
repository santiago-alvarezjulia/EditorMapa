#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "Observador.h"

class LabelMapa : public QLabel {
    public:
	    LabelMapa(const QString& text, std::string id, QWidget * parent = 0);
        void agregar_observador(Observador* observador);
        void actualizar_imagen(const QString& path_imagen);
        ~LabelMapa();
 
    signals:
	    void clickeado();
 
    protected:
	    void mousePressEvent(QMouseEvent * event);

    private:
        std::string id;
        Observador* observador;
};

#endif // LABEL_H