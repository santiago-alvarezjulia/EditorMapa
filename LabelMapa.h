#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "ObservadorMapa.h"

class LabelMapa : public QLabel {
    public:
	    LabelMapa(QPixmap& terrenos, std::string id, int pos_x, int pos_y, 
            std::string tipo, std::string pos_label, QWidget * parent = 0);
        void agregar_observador(ObservadorMapa* observador);
        void actualizar_data(QPixmap& nueva_imagen, int nueva_posicion_x,
            int nueva_posicion_y, std::string nuevo_tipo);
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
        int posicion_x;
        int posicion_y;
        std::string tipo;
        std::string pos_label;
        QPixmap& terrenos;
        ObservadorMapa* observador;
};

#endif // LABEL_H