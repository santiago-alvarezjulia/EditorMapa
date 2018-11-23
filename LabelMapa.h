#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "ObservadorMapa.h"

class LabelMapa : public QLabel {
    public:
        LabelMapa(QPixmap& terrenos, std::string id, int tipo, 
            std::string pos_label, QWidget* parent = 0);
        void agregar_observador(ObservadorMapa* observador);
        
        void actualizar_data(QPixmap& nueva_imagen, int nuevo_tipo, 
            std::string nuevo_id);
        void actualizar_imagen(QPixmap& nueva_imagen);
        
        void set_marco_mouse_enter();
        void borrar_marco_mouse_enter();

        void agregar_imagen_jugador(QPixmap& sprite_jugador);
        
        int get_tipo();
        std::string get_id();

        ~LabelMapa();
 
    signals:
	    void clickeado();
 
    protected:
	    void mousePressEvent(QMouseEvent * event);
        void enterEvent(QEvent* event);
        void leaveEvent(QEvent* event);

    private:
        int tipo;
        std::string id;
        std::string pos_label;
        QPixmap& terrenos;
        ObservadorMapa* observador;
};

#endif // LABEL_H