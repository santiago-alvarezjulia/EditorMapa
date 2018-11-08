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
};

#endif // EDITOR_H