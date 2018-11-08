#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "Tabs.h"
#include "Mapa.h"
#include "Observador.h"

class Editor : public QWidget, Observador {
    public:
        explicit Editor(QWidget *parent = 0);
        virtual void en_notificacion(std::string id_label);
        ~Editor();
        
    private:
        Mapa* mapa;
        Tabs* tabs;
};

#endif // EDITOR_H