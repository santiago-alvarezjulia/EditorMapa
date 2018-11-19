#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "ObservadorMapa.h"

class LabelMapa : public QLabel {
    public:
        LabelMapa(QPixmap& terrenos, std::string id, std::string tipo, 
            std::vector<uint32_t> pos_tiles, std::string pos_label, 
            QWidget* parent = 0);
        void agregar_observador(ObservadorMapa* observador);
        void actualizar_data(QPixmap& nueva_imagen, 
            std::vector<uint32_t> nuevas_pos_tiles, std::string nuevo_tipo);
        void set_marco_mouse_enter();
        void borrar_marco_mouse_enter();
        std::string get_tipo();
        ~LabelMapa();
 
    signals:
	    void clickeado();
 
    protected:
	    void mousePressEvent(QMouseEvent * event);
        void enterEvent(QEvent* event);
        void leaveEvent(QEvent* event);

    private:
        std::string id;
        std::vector<uint32_t> pos_tiles;
        std::string tipo;
        std::string pos_label;
        QPixmap& terrenos;
        ObservadorMapa* observador;
};

#endif // LABEL_H