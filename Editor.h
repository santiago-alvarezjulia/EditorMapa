#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "Tabs.h"
#include "Mapa.h"
#include "Observador.h"
#include <QSpinBox>
#include <QMenuBar>

class Editor : public QWidget, Observador {
    public:
        /**
         * \brief 1er Constructor de Editor.
         * 
         * Constructor de Editor que es utilizado cuando se crea un mapa. Recibe como 
         * parametros del mapa el tamaño (filas y columnas) y la cantidad de jugadores.
         */
        Editor(int filas, int columnas, int cant_elegida_jugadores, 
            QWidget *parent = 0);

        /**
         * \brief 2do Constructor de Editor.
         * 
         * Constructor de Editor que es utilizado cuando se carga un mapa. Recibe como
         * parametro el nombre del archivo del mapa. El archivo contiene la informacion
         * relacionada con el tamaño (filas y columnas) y la cantidad de jugadores.
         */
        Editor(std::string& filename_json, QWidget *parent = 0);

        /**
         * \brief Metodo virtual de la interfaz Observador implementada por Editor.
         * 
         * Metodo virtual que es llamado por Mapa cuando un label del mapa (LabelMapa)
         * es clickeado. Recibe por parametro el id del LabelMapa.
         */
        virtual void en_notificacion(std::string& id_label_mapa);

        /**
         * \brief Destructor de Editor.
         * 
         * Destructor de editor.
         */
        ~Editor();
        
    private:
        Mapa mapa;
        Tabs tabs;
        QSpinBox* spin_box_cantidad_jugadores;
        QMenuBar* menu_bar;
        int cant_elegida_jugadores;
        int min_cant_jugadores;
        
        /**
         * \brief Inicializa la interfaz del Editor.
         * 
         * Asocia los botones con sus respectivos metodos y el resto de widgets
         * presentes en la ui del Editor. Agrego el menu bar.
         */
        void inicializar_menu();

        /**
         * \brief Funcion que guarda el mapa en un archivo.
         * 
         * Antes de guardar el mapa, verifica que sea valido (se fija en la cantidad
         * de jugadores ubicados en el mapa) y luego muestra un dialogo para elegir
         * el nombre del mapa y el directorio. Finalmente delega en Mapa la generacion
         * del archivo.
         */
        void guardar_mapa();

        void cargar_mapa_en_ejecucion();
        
        void mostrar_dialogo_tamanio_mapa();

        void mostrar_dialogo_cantidad_jugadores();

        void terminar_ejecucion();
};

#endif // EDITOR_H