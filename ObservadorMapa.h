#ifndef OBSERVADOR_MAPA_H
#define OBSERVADOR_MAPA_H
#include <string>

class ObservadorMapa {
    public:
        virtual ~ObservadorMapa() {}
        virtual void label_mapa_clickeado(std::string id_label_mapa) = 0;
        virtual void label_mapa_enter_event(std::string id_label_mapa) = 0;
        virtual void label_mapa_leave_event(std::string id_label_mapa) = 0;
};

#endif // OBSERVADOR_MAPA_H