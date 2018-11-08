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
        virtual void en_notificacion(std::string id_label);

    private:
        QWidget* parent;
        std::map<std::string, Label*> tab_terrenos;
        std::map<std::string, Label*> tab_infanteria;
        std::map<std::string, Label*> tab_vehiculos;
        std::string id_label_clickeado;

        void agregar_terrenos();
        void agregar_infanteria();
        void agregar_vehiculos();
};

#endif // TABS_H