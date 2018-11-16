#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "Tabs.h"
#include "Mapa.h"
#include "ObservadorMapa.h"

class Editor : public QWidget, ObservadorMapa {
    public:
        explicit Editor(QWidget *parent = 0);
        virtual void label_mapa_clickeado(std::string id_label_mapa);
        virtual void label_mapa_enter_event(std::string id_label_mapa);
        virtual void label_mapa_leave_event(std::string id_label_mapa);
        ~Editor();
        
    private:
        Mapa* mapa;
        Tabs* tabs;
        void mostrar_dialog_bienvenida();
        void mostrar_dialog_dimension_mapa();
        void conectar_botones();
        void guardar_mapa();
        void cargar_mapa();
};

#endif // EDITOR_H