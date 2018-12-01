#ifndef MANEJADOR_JSON_H
#define MANEJADOR_JSON_H

#include <string>
#include <vector>
#include <map>
#include "LabelMapa.h"

class ManejadorJson {
    public: 
        ManejadorJson(){};

        void generar_json(std::string& nombre_archivo, int filas, int columnas,
            std::map<std::string, LabelMapa*>& mapa, 
            std::map<std::string, bool>& jugadores);

        ~ManejadorJson(){};
        
    private:
        /**
         * \brief Split de string.
         * 
         * Devuelvo un vector de strings a partir del splitteo de un string y un 
         * caracter delimitador.
         */
        std::vector<std::string> split(const std::string& str, char delim);
};

#endif // MANEJADOR_JSON_H