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
        Mapa(std::string filename_json, QWidget* parent = 0);
        void actualizar_data(std::string id_label, QPixmap& nueva_imagen, 
            std::vector<uint32_t> nuevas_pos_tiles, std::string nuevo_tipo);
        void agregar_observador(Observador* observer);
        virtual void label_mapa_clickeado(std::string id_label_mapa);
        virtual void label_mapa_enter_event(std::string id_label_mapa);
        virtual void label_mapa_leave_event(std::string id_label_mapa);

        bool es_valido();
        void generar_json(std::string nombre_archivo);

        void inicializar_mapa();

        ~Mapa();

    private:
        int filas;
        int columnas;
        QWidget* parent;
        QPixmap imagen_terrenos;
        Observador* observador;
        std::map<std::string, LabelMapa*> mapa;
        void parsear_json(std::string filename_json);
        std::vector<std::string> split(const std::string& str, char delim);
};

#endif // MAPA_H