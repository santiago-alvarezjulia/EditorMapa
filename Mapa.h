#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <map>
#include "LabelMapa.h"
#include "Observador.h"

class Mapa : public Observador {
    public:
        Mapa(QWidget* parent = 0);
        virtual void en_notificacion(std::string id_label);

    private:
        QWidget* parent;
        std::map<std::string, LabelMapa*> mapa;
        void inicializar_mapa();
};

#endif // MAPA_H