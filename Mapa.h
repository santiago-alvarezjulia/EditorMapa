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
        Mapa(QWidget* parent = 0);
        
        void actualizar_data(std::string id_label, QPixmap& nueva_imagen, 
            std::vector<uint32_t> nuevas_pos_tiles, std::string nuevo_tipo);
        void agregar_jugador(std::string id_label, QPixmap& nueva_imagen);
        void agregar_observador(Observador* observer);
        virtual void label_mapa_clickeado(std::string id_label_mapa);
        virtual void label_mapa_enter_event(std::string id_label_mapa);
        virtual void label_mapa_leave_event(std::string id_label_mapa);
        std::string get_tipo_by_id(std::string id_label_mapa);

        void parsear_json(std::string filename_json);
        void generar_json(std::string nombre_archivo);

        void inicializar_mapa();

        int get_cantidad_jugadores_agregados();
        int get_cantidad_jugadores();

        bool es_valido_agregar_jugador(std::string id_label_mapa);

        ~Mapa();

    private:
        int filas;
        int columnas;
        int cantidad_jugadores;
        QWidget* parent;
        QPixmap imagen_terrenos;
        Observador* observador;
        std::map<std::string, LabelMapa*> mapa;
        std::map<std::string, bool> jugadores;
        std::vector<std::vector<uint32_t>> reacomodar_sprites_json(
            std::vector<std::vector<uint32_t>> sprites); 
        std::vector<std::string> split(const std::string& str, char delim);
};

#endif // MAPA_H