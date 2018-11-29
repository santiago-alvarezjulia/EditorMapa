#ifndef TABS_H
#define TABS_H

#include <string>
#include <map>
#include <vector>
#include "LabelTab.h"
#include "Observador.h"

class Tabs : public Observador {
    public:
        /**
         * \brief Constructor Tabs.
         * 
         * Es llamado por el constructor de Editor, por lo tanto se separa la creacion
         * de la inicializacion de Tabs (el Editor debe hacer setupUi para poder 
         * utilizar los elementos graficos de Qt como QGridLayout).
         */
        explicit Tabs(QWidget* parent = 0);

        /**
         * \brief Inicializador Tabs.
         * 
         * Llena las pestañas o tabs de sprites. Los separa segun su tipo.
         */
        void inicializar_tabs();

        /**
         * \brief Getter id label clickeado.
         * 
         * Devuelvo el id del label clickeado, puede estar vacio ("").
         */
        std::string get_id_label_clickeado();

        /**
         * \brief Getter tipo del label clickeado.
         * 
         * Devuelvo el tipo del label clickeado (delego en LabelTab). Precondicion -> tiene que haberse
         * verificado que hay un label clickeado.
         */
        int get_tipo_label_clickeado();

        /**
         * \brief Getter imagen del label clickeado.
         * 
         * Devuelvo la imagen del label clickeado (delego en LabelTab). 
         * Precondicion -> tiene que haberse verificado que hay un label clickeado.
         */
        QPixmap get_imagen_clickeado();

        /**
         * \brief Metodo virtual de la interfaz Observador implementada por Tabs.
         * 
         * Metodo virtual que es llamado cuando un label de una pestaña (LabelTab)
         * es clickeado. Recibe por parametro el id del Label.
         */
        virtual void en_notificacion(std::string& id_label);

        /**
         * \brief Destructor de Tabs.
         * 
         * Libero los Label del heap (fueron tomados los recursos en 
         *  Tabs::inicializar_tabs)
         */
        ~Tabs();

    private:
        QWidget* parent;
        QPixmap imagen_terrenos;
        std::map<std::string, LabelTab*> tabs_terrenos;
        std::string id_label_clickeado;
};

#endif // TABS_H