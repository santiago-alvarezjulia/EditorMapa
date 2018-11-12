#ifndef TABS_H
#define TABS_H

#include <string>
#include <map>
#include "Label.h"
#include "Observador.h"

class Tabs : public Observador {
    public:
        Tabs(QWidget* parent = 0);
        std::string get_id_label_clickeado();
        int get_alto_label_clickeado();
        int get_ancho_label_clickeado();
        QPixmap get_imagen_clickeado();
        virtual void en_notificacion(std::string id_label);
        ~Tabs();

    private:
        QWidget* parent;
        QPixmap imagen_terrenos;
        std::map<std::string, Label*> tab_terrenos;
        std::string id_label_clickeado;

        void agregar_terrenos();
};

#endif // TABS_H