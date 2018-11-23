#ifndef LABEL_TAB_H
#define LABEL_TAB_H

#include <QWidget>
#include <QLabel>
#include <string>
#include <vector>
#include "Observador.h"

class LabelTab : public QLabel {
    public:
        LabelTab(QPixmap& terrenos, std::string id, int tipo, 
            std::vector<uint32_t> pos_tiles , QWidget* parent = 0);
        int get_tipo();
        std::string get_id();
        QPixmap get_imagen();
	    void set_marco_clickeado();
        void borrar_marco_clickeado();
        void agregar_observador(Observador* observador);
        ~LabelTab();
 
    signals:
	    void clickeado();
 
    protected:
	    void mousePressEvent(QMouseEvent * event);

    private:
        std::string id;
        int tipo;
        QPixmap pixmap;
        Observador* observador;
};

#endif // LABEL_TAB_H
