#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "Tabs.h"
#include "Mapa.h"
#include "ObservadorMapa.h"
#include <QSpinBox>

class Editor : public QWidget, Observador {
    public:
        Editor(int filas, int columnas, int cant_jugadores, 
            QWidget *parent = 0);
        Editor(std::string filename_json, QWidget *parent = 0);
        virtual void en_notificacion(std::string id_label_mapa);
        ~Editor();
        
    private:
        Mapa mapa;
        Tabs tabs;
        QSpinBox* spin_box_cantidad_jugadores;
        int cant_jugadores;
        void conectar_botones();
        void guardar_mapa();
        void cambiar_tamanio_mapa();
        void cambiar_cantidad_jugadores();
};

#endif // EDITOR_H