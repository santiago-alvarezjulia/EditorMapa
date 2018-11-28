#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "ObservadorMapa.h"

class LabelMapa : public QLabel {
    public:
        /**
         * \brief Constructor de LabelMapa.
         * 
         * Constructor de LabelMapa que recibe como parametro el QPixmap correspondiente. 
         * Tambien el pos_label, el tipo y la posicion de los tiles de este 
         * LabelMapa en particular.
         */
        LabelMapa(QPixmap& terrenos, std::string id, int tipo, 
            std::string pos_label, QWidget* parent = 0);

        /**
         * \brief Agrego observador del LabelMapa.
         * 
         * Agrego un observador del LabelMapa (Mapa).
         */
        void agregar_observador(ObservadorMapa* observador);
        
        /**
         * \brief Actualizar data del LabelMapa.
         * 
         * Actualizo la data del LabelMapa.
         */
        void actualizar_data(QPixmap& nueva_imagen, int nuevo_tipo, 
            std::string nuevo_id);

        /**
         * \brief Actualizar imagen del LabelMapa.
         * 
         * Actualizo la imagen del LabelMapa.
         */
        void actualizar_imagen(QPixmap& nueva_imagen);
        
        /**
         * \brief Agrego el marco clickeado.
         * 
         * Agrego el marco clickeado.
         */
        void set_marco_mouse_enter();

        /**
         * \brief Borro el marco clickeado.
         * 
         * Borro el marco clickeado.
         */
        void borrar_marco_mouse_enter();

        /**
         * \brief Getter tipo del LabelMapa.
         * 
         * Devuelvo el tipo del LabelMapa.
         */
        int get_tipo();

        /**
         * \brief Getter id del LabelMapa.
         * 
         * Devuelvo el id del LabelMapa.
         */
        std::string get_id();

        /**
         * \brief Destructor de LabelMapa.
         * 
         * Destructor de LabelMapa.
         */
        ~LabelMapa() {};
 
    protected:
        /**
         * \brief Atrapo mouse clicks sobre el LabelMapa.
         * 
         * Le aviso al observador que fue clickeado. 
         */
	    void mousePressEvent(QMouseEvent * event);

        /**
         * \brief Atrapo entradas del mouse sobre el LabelMapa.
         * 
         * Le aviso al observador que el mouse entro al LabelMapa. 
         */
        void enterEvent(QEvent* event);

        /**
         * \brief Atrapo salidas del mouse del LabelMapa.
         * 
         * Le aviso al observador que el mouse salio del LabelMapa. 
         */
        void leaveEvent(QEvent* event);

    private:
        int tipo;
        std::string id;
        std::string pos_label;
        QPixmap& terrenos;
        ObservadorMapa* observador;
};

#endif // LABEL_H