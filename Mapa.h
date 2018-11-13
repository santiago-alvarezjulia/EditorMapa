#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <map>
#include <vector>
#include "LabelMapa.h"
#include "ObservadorMapa.h"

class Mapa : public ObservadorMapa {
    public:
        Mapa(QWidget* parent = 0);
        void actualizar_data(std::string id_label, QPixmap& nueva_imagen, 
            int nueva_posicion_x, int nueva_posicion_y, std::string nuevo_tipo);
        void set_marco_mouse_enter(std::string id_label);
        void borrar_marco_mouse_enter(std::string id_label);
        void agregar_observador(ObservadorMapa* observer);
        virtual void label_mapa_clickeado(std::string id_label_mapa);
        virtual void label_mapa_enter_event(std::string id_label_mapa);
        virtual void label_mapa_leave_event(std::string id_label_mapa);

        ~Mapa();

    private:
        QWidget* parent;
        ObservadorMapa* observador;
        std::map<std::string, LabelMapa*> mapa;
        void inicializar_mapa();
        std::vector<std::string> split(const std::string& str, char delim);
};

#endif // MAPA_H