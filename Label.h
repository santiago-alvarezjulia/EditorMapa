#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>
#include <string>
#include <vector>
#include "Observador.h"

class Label : public QLabel {
    public:
        Label(QPixmap& terrenos, std::string id, std::string tipo, 
            std::vector<uint32_t> pos_tiles , QWidget* parent = 0);
        std::vector<uint32_t> get_pos_tiles();
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
        std::vector<uint32_t> pos_tiles;
        QPixmap pixmap;
        Observador* observador;
};

#endif // LABEL_H
