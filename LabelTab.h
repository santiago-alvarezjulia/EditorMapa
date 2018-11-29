#ifndef LABEL_TAB_H
#define LABEL_TAB_H

#include <QWidget>
#include <QLabel>
#include <string>
#include <vector>
#include "Observador.h"

class LabelTab : public QLabel {
    public:
        /**
         * \brief Constructor de LabelTab.
         * 
         * Constructor de LabelTab que recibe como parametro el QPixmap correspondiente
         * a la imagen .bmp que contiene todos los sprites de los terrenos. Tambien
         * el id, el tipo y la posicion de los tiles de este Label en particular.
         */
        LabelTab(QPixmap& terrenos, std::string& id, int tipo, 
            std::vector<uint32_t>& pos_tiles , QWidget* parent = 0);

        /**
         * \brief Getter tipo del LabelTab.
         * 
         * Devuelvo el tipo del LabelTab.
         */
        int get_tipo();

        /**
         * \brief Getter id del LabelTab.
         * 
         * Devuelvo el id del LabelTab.
         */
        std::string get_id();

        /**
         * \brief Getter imagen del LabelTab.
         * 
         * Devuelvo la imagen del LabelTab.
         */
        QPixmap get_imagen();
	    
        /**
         * \brief Agrego observador del LabelTab.
         * 
         * Agrego un observador del LabelTab (Tabs).
         */
        void agregar_observador(Observador* observador);

        /**
         * \brief Agrego el marco clickeado.
         * 
         * Agrego el marco clickeado.
         */
        void set_marco_clickeado();

        /**
         * \brief Borro el marco clickeado.
         * 
         * Borro el marco clickeado.
         */
        void borrar_marco_clickeado();
        
        /**
         * \brief Destructor de Label.
         * 
         * Destructor de Label.
         */
        ~LabelTab() {};
 
    protected:
        /**
         * \brief Atrapo mouse clicks sobre el LabelTab.
         * 
         * Le aviso al observador que fue clickeado.
         */
	    void mousePressEvent(QMouseEvent * event);

    private:
        std::string id;
        int tipo;
        QPixmap pixmap;
        Observador* observador;
};

#endif // LABEL_TAB_H
