#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <map>
#include <vector>
#include "LabelMapa.h"
#include "Observador.h"
#include "ObservadorMapa.h"

struct Sprite {
    int tipo;
    QPixmap sprite;
};

class Mapa : public ObservadorMapa {
    public:
        Mapa(int filas, int columnas, QWidget* parent = 0);
        Mapa(QWidget* parent = 0);
        
        void actualizar_data(std::string id_label, QPixmap& nueva_imagen, 
            int nuevo_tipo, std::string nuevo_id);
        void agregar_jugador(std::string id_label, QPixmap& nueva_imagen);
        void agregar_observador(Observador* observer);
        virtual void label_mapa_clickeado(std::string id_label_mapa);
        
        int get_tipo_by_id(std::string id_label_mapa);

        void parsear_json(std::string filename_json);
        void generar_json(std::string nombre_archivo);

        void inicializar_mapa();

        int get_cantidad_jugadores_agregados();

        bool es_valido_agregar_jugador(std::string id_label_mapa, 
            int cantidad_jugadores);

        void cambiar_tamanio(int nueva_cant_filas, int nueva_cant_columnas);

        ~Mapa();

    private:
        int filas;
        int columnas;
        QWidget* parent;
        QPixmap imagen_terrenos;
        Observador* observador;
        std::map<std::string, LabelMapa*> mapa;
        std::map<std::string, bool> jugadores;

        std::vector<std::string> split(const std::string& str, char delim);
        
        QPixmap generar_sprite_inicial(std::vector<uint32_t> pos_tiles);
        std::map<std::string, Sprite> generar_sprites_posibles();

        void agrandar_mapa(int nueva_cant_filas, int nueva_cant_columnas);
        void achicar_mapa(int nueva_cant_filas, int nueva_cant_columnas);
        void sacar_columnas_agregar_filas(int nueva_cant_filas, 
            int nueva_cant_columnas);
        void sacar_filas_agregar_columnas(int nueva_cant_filas, 
            int nueva_cant_columnas);
};

#endif // MAPA_H