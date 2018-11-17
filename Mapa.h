#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <map>
#include <vector>
#include "LabelMapa.h"
#include "Observador.h"
#include "ObservadorMapa.h"

class Mapa : public ObservadorMapa {
    public:
        Mapa(int filas, int columnas, QWidget* parent = 0);
        void actualizar_data(std::string id_label, QPixmap& nueva_imagen, 
            int nueva_posicion_x, int nueva_posicion_y, std::string nuevo_tipo);
        void agregar_observador(Observador* observer);
        virtual void label_mapa_clickeado(std::string id_label_mapa);
        virtual void label_mapa_enter_event(std::string id_label_mapa);
        virtual void label_mapa_leave_event(std::string id_label_mapa);

        bool es_valido();
        void generar_json();

        void inicializar_mapa();

        ~Mapa();

    private:
        int filas;
        int columnas;
        QWidget* parent;
        QPixmap imagen_terrenos;
        Observador* observador;
        std::map<std::string, LabelMapa*> mapa;
        std::vector<std::string> split(const std::string& str, char delim);
};

#endif // MAPA_H