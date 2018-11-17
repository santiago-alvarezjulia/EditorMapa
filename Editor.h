#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "Tabs.h"
#include "Mapa.h"
#include "ObservadorMapa.h"

class Editor : public QWidget, Observador {
    public:
        Editor(int filas, int columnas, int cant_jugadores, 
            QWidget *parent = 0);
        virtual void en_notificacion(std::string id_label_mapa);
        ~Editor();
        
    private:
        Mapa mapa;
        Tabs tabs;
        int cant_jugadores;
        void conectar_botones();
        void guardar_mapa();
};

#endif // EDITOR_H