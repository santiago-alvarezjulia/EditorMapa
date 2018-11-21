#ifndef TABS_H
#define TABS_H

#include <string>
#include <map>
#include <vector>
#include "LabelTab.h"
#include "Observador.h"

class Tabs : public Observador {
    public:
        Tabs(QWidget* parent = 0);
        void inicializar_tabs();
        std::string get_id_label_clickeado();
        std::vector<uint32_t> get_pos_tiles_clickeado();
        int get_tipo_label_clickeado();
        QPixmap get_imagen_clickeado();
        virtual void en_notificacion(std::string id_label);
        ~Tabs();

    private:
        QWidget* parent;
        QPixmap imagen_terrenos;
        std::map<std::string, LabelTab*> tabs_terrenos;
        std::string id_label_clickeado;
};

#endif // TABS_H