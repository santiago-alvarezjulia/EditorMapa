#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "ObservadorMapa.h"

class LabelMapa : public QLabel {
    public:
	    LabelMapa(const QString& text, std::string id, QWidget * parent = 0);
        void agregar_observador(ObservadorMapa* observador);
        void actualizar_imagen(const QString& path_imagen);
        void set_marco_mouse_enter();
        void borrar_marco_mouse_enter();
        ~LabelMapa();
 
    signals:
	    void clickeado();
 
    protected:
	    void mousePressEvent(QMouseEvent * event);
        void enterEvent(QEvent* event);
        void leaveEvent(QEvent* event);

    private:
        std::string id;
        ObservadorMapa* observador;
};

#endif // LABEL_H