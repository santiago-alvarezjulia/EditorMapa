#include "Mapa.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "LabelMapa.h"
#include "libs/json.hpp"
#include <sstream>
#include <QMessageBox>
#include <fstream>
#include <QPainter>
#define DELIM_ID ','
using std::string;
using std::stringstream;
using std::getline;
using std::vector;
using std::map;
using nlohmann::json;

/**
 * \brief 1er Constructor de Mapa.
 * 
 * Constructor de Mapa que es utilizado cuando se crea un mapa desde 0. 
 * Recibe como parametros el tamaño (filas y columnas) y la cantidad de jugadores.
 */
Mapa::Mapa(int filas, int columnas, QWidget* parent) : filas(filas), 
    columnas(columnas), parent(parent) {
    this->mapa = map<string, LabelMapa*>();
    this->jugadores = map<string, bool>();
    // hardcodeo el nombre del archivo .bmp con los sprites del terreno.
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
}

/**
 * \brief 2do Constructor de Mapa.
 * 
 * Constructor de Mapa que es utilizado cuando se carga un mapa. El tamaño y la
 * cantidad de jugadores se define en Mapa::parsear_json.
 */
Mapa::Mapa(QWidget* parent) : parent(parent) {
    this->mapa = std::map<std::string, LabelMapa*>();
    this->jugadores = map<string, bool>();
    // hardcodeo el nombre del archivo .bmp con los sprites del terreno.
    this->imagen_terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");
}

/**
 * \brief Parseo del json Mapa.
 * 
 * Parseo del json con la informacion de un mapa creado y almacenado anteriormente.
 * Precondicion -> haber construido Mapa con el 2do Constructor de Mapa.
 * 
 */
void Mapa::parsear_json(string filename_json) {
    // getteo el layout y el widget del Mapa
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos
    std::ifstream entrada(filename_json);
    json mapa_json;
    entrada >> mapa_json;

    vector<vector<uint32_t>> pos_jugadores = mapa_json["jugadores"];
    vector<vector<string>> ids = mapa_json["tipo"];
    int cant_fila = ids.size();
    this->filas = cant_fila;

    map<string, Sprite> sprites_posibles = generar_sprites_posibles();
    
    vector<vector<string>>::iterator it_filas_ids = ids.begin();
    int cant_columnas = (*it_filas_ids).size();
    this->columnas = cant_columnas;

    int fila_actual = 0;
    int columna_actual = 0;
    for (; it_filas_ids != ids.end(); ++it_filas_ids) {
        vector<string>::iterator it_ids = (*it_filas_ids).begin();
        for (; it_ids != (*it_filas_ids).end(); ++it_ids) {
            string id = *it_ids;
            
            string pos_label ("");
            pos_label += std::to_string(fila_actual);
            pos_label += DELIM_ID;
            pos_label += std::to_string(columna_actual);
            
            Sprite sprite;
            map<string, Sprite>::iterator it_sprites = sprites_posibles.find(id);
            if (it_sprites != sprites_posibles.end()) {
                sprite = it_sprites->second;
            }
            
            LabelMapa* label_mapa = new LabelMapa(sprite.sprite, id, sprite.tipo,
                pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, fila_actual, columna_actual);
            
            this->mapa.emplace(pos_label, label_mapa);
            
            columna_actual++;
            
            if (columna_actual == cant_columnas) {
                columna_actual = 0;
                fila_actual++;
            } 

        }
    }

    // hardcodeo jugador1 como id
    Sprite sprite;
    map<string, Sprite>::iterator it_sprites = sprites_posibles.find("jugador1");
    if (it_sprites != sprites_posibles.end()) {
        sprite = it_sprites->second;
    
    }
    vector<vector<uint32_t>>::iterator it_pos_jugadores = pos_jugadores.begin();
    for (; it_pos_jugadores != pos_jugadores.end(); ++it_pos_jugadores) {
        string id_label ("");
        id_label += std::to_string((*it_pos_jugadores)[0]);
        id_label += ',';
        id_label += std::to_string((*it_pos_jugadores)[1]);
        this->jugadores.emplace(id_label, true);
        map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	    if (it != this->mapa.end()) {
            it->second->actualizar_imagen(sprite.sprite);
        } 
    }

    this->cantidad_jugadores = pos_jugadores.size();

    map_layout->setSpacing(0);
    scroll_area_mapa->setLayout(map_layout);
}

map<string, Sprite> Mapa::generar_sprites_posibles() {
    map<string, Sprite> sprites_posibles;
    QPixmap terrenos = QPixmap ("../sprites/terrain/d2k_BLOXBASE.bmp");

    // hardcodeo la ubicacion del archivo .json con la informacion sobre los
    // sprites del archivo de terrenos (QPixmap terrenos).
    std::ifstream entrada("../sprites/terrain/terrenos.json");

    json terrenos_json;

    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for (; it != terrenos_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        // valores donde ubicar los label dentro de los gridLayout de cada pestaña.
        int columna = 0;
        int fila = 0;

        auto it_sprites = elem["sprites"].begin();
        for (int i = 0; it_sprites != elem["sprites"].end(); ++it_sprites) {
            json tile = *it;

            Sprite sprite;
            sprite.tipo = elem["tipo"];
            vector<uint32_t> pos_tiles = tile["sprites"][i]["pos_tiles"];
            string id = tile["sprites"][i]["id"];

            QPixmap label_32_x_32 (32, 32);

            // junto los 16 tiles de 8x8 pixeles, cuyas posiciones se encuentran en 
            // pos_tiles y los dibujo en un unico QPixmap de 32x32 pixeles. 
            // pos_x_label y pos_y_label es la posicion de cada sprite de 8x8 dentro
            // del QPixmap de 32x32.
            int pos_x_label = 0;
            int pos_y_label = 0;
            vector<uint32_t>::iterator it_pos_tiles = pos_tiles.begin();
            for (int cont_tiles = 0; it_pos_tiles != pos_tiles.end(); ++it_pos_tiles) {
                // 8 es el tamaño individual de cada sprite (8x8 pixeles).
                // 20 es el ancho del archivo .bmp que contiene todos los sprites 
                // de los terrenos.
                // x e y son las posiciones de cada sprite de 8x8 en el archivo .bmp .
                // los -1 es que arranco a contar desde 1.
                int y = ((*it_pos_tiles - 1) / 20) * 8;
                int x;
                if (*it_pos_tiles < 20) {
                    x = (*it_pos_tiles - 1) * 8;
                } else {
                    x = ((*it_pos_tiles - 1) % 20) * 8;
                }

                // copio el cuadrado de 8x8 que quiero del .bmp .
                QRect rect(x, y, 8, 8);
                QPixmap cropped = terrenos.copy(rect);
            
                QPainter painter (&label_32_x_32);
                if (cont_tiles < 4) {
                    pos_x_label = cont_tiles * 8;
                } else {
                    pos_x_label = (cont_tiles % 4) * 8;
                }

                pos_y_label = (cont_tiles / 4) * 8;
                painter.drawPixmap(pos_x_label, pos_y_label, 8, 8, cropped);

                cont_tiles++;
            }

            sprite.sprite = label_32_x_32;

            sprites_posibles.emplace(id, sprite);
        }
    }

    return sprites_posibles;
}

/**
 * \brief Inicializacion Mapa.
 * 
 * Precondicion -> haber construido Mapa con el 1er Constructor de Mapa.
 */
void Mapa::inicializar_mapa() {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos.
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& data_general = *it;
    
    ++it;
    const json& elem = *it;

    auto it_tiles = elem["sprites"].begin();
    json tile = *it;
    string id = tile["sprites"][0]["id"];
    int tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];
    QPixmap sprite_inicial = generar_sprite_inicial(pos_tiles);

    for (int i = 0; i < this->filas; ++i) {
        for (int j = 0; j < this->columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, id, tipo,
                pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    map_layout->setSpacing(0);
    scroll_area_mapa->setLayout(map_layout);
}

QPixmap Mapa::generar_sprite_inicial(vector<uint32_t> pos_tiles) {
    QPixmap label_32_x_32 (32, 32);

    // junto los 16 tiles de 8x8 pixeles, cuyas posiciones se encuentran en 
    // pos_tiles y los dibujo en un unico QPixmap de 32x32 pixeles. 
    int pos_x_label = 0;
    int pos_y_label = 0;
    vector<uint32_t>::iterator it_pos_tiles = pos_tiles.begin();
    for (int cont_tiles = 0; it_pos_tiles != pos_tiles.end(); ++it_pos_tiles) {
        // 8 es el tamaño individual de cada sprite (8x8 pixeles).
        // 20 es el ancho del archivo .bmp que contiene todos los sprites 
        // de los terrenos.
        // x e y son las posiciones de cada sprite de 8x8 en el archivo .bmp .
        int y = ((*it_pos_tiles - 1) / 20) * 8;
        int x;
        if (*it_pos_tiles < 20) {
            x = ((*it_pos_tiles) - 1) * 8;
        } else {
            x = ((*it_pos_tiles - 1) % 20) * 8;
        }

        // copio el cuadrado de 8x8 que quiero del .bmp .
        QRect rect(x, y, 8, 8);
        QPixmap cropped = this->imagen_terrenos.copy(rect);
    
        // pos_x_label y pos_y_label es la posicion de cada sprite de 8x8 dentro
        // del QPixmap de 32x32.
        QPainter painter (&label_32_x_32);
        if (cont_tiles < 4) {
            pos_x_label = cont_tiles * 8;
        } else {
             pos_x_label = (cont_tiles % 4) * 8;
        }

        pos_y_label = (cont_tiles / 4) * 8;
        painter.drawPixmap(pos_x_label, pos_y_label, 8, 8, cropped);

        cont_tiles++;
    }

    return label_32_x_32;
}

/**
 * \brief Getter cantidad de jugadores.
 * 
 * Devuelvo la cantidad de jugadores que debe tener el mapa (lo proporciona el
 * usuario de la aplicación en DialogoBienvenida).
 */
int Mapa::get_cantidad_jugadores() {
    return this->cantidad_jugadores;
}

/**
 * \brief Getter tipo de LabelMapa.
 * 
 * Devuelvo el tipo del LabelMapa cuyo id es el especificado en el parámetro 
 * (delego en LabelMapa). Precondicion -> el id_label_mapa es correcto.
 */
int Mapa::get_tipo_by_id(string id_label_mapa) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label_mapa);
	if (it != this->mapa.end()) {
        return it->second->get_tipo();
    }
}

/**
 * \brief Validez agregar jugador.
 * 
 * Verifico que no haya un jugador en el LabelMapa cuyo id recibo por parametro.
 */
bool Mapa::es_valido_agregar_jugador(string id_label_mapa, 
    int cantidad_jugadores) {
    // tipo Roca (0)
    if (this->get_tipo_by_id(id_label_mapa) == 0) {
         // me fijo si faltan ubicar jugadores o ya fueron todos ubicados
        if (this->get_cantidad_jugadores_agregados() < cantidad_jugadores) {
            // me fijo si ya hay un jugador en esa posicion del Mapa
            if (this->jugadores.find(id_label_mapa) == this->jugadores.end()) {
                return true;
            } else {
                QMessageBox::critical(0, "Error al agregar un jugador", 
                    "Ya agregaste un jugador en esa posicion");
                return false;
            }          
        } else {
            QMessageBox::critical(0, "Error al agregar un jugador", 
                "Ya alcanzaste el maximo de jugadores posibles");
            return false;
        }    
    } else {
        QMessageBox::critical(0, "Error al agregar un jugador", 
            "Solo se puede agregar jugadores sobre roca");
        return false;
    }
}

/**
 * \brief Genero json de Mapa.
 * 
 * Genero json con la informacion de Mapa. Recibo el nombre del archivo que tengo 
 * que generar por parametro (filepath incluido).
 */
void Mapa::generar_json(string nombre_archivo) {
    json j;
    
    vector<vector<string>> tipos;
    vector<vector<uint32_t>> jugadores_json;
    int cont_sprites_agregados = 0;

    for (int i = 0; i < this->filas; ++i) {
        vector<string> tipos_por_columna;
        
        for (int j = 0; j < this->columnas; ++j) {
            string id_label ("");
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	        if (it != this->mapa.end()) {
                tipos_por_columna.emplace_back(it->second->get_id());
            }
        }

        tipos.emplace_back(tipos_por_columna);
    }

    vector<uint32_t> pos_jugadores;
    map<string, bool>::iterator it_jugadores = this->jugadores.begin();
    for (; it_jugadores != this->jugadores.end(); ++it_jugadores) {
        vector<string> splitteado = this->split(it_jugadores->first, DELIM_ID);
        pos_jugadores.emplace_back(std::stoi(splitteado[0]));
        pos_jugadores.emplace_back(std::stoi(splitteado[1]));
        jugadores_json.emplace_back(pos_jugadores);
        pos_jugadores.clear();
    }

    // agrego data al json
    j["tipo"] = tipos;
    j["jugadores"] = jugadores_json;

    // genero el archivo mapa.json
    std::ofstream file(nombre_archivo);
    file << j;
}

/**
 * \brief Agrego observador del Mapa.
 * 
 * Agrego un observador del Mapa (Editor).
 */
void Mapa::agregar_observador(Observador* observer) {
    this->observador = observer;
}

/**
 * \brief Actualizar data.
 * 
 * Actualizo la data de un LabelMapa cuyo id recibo por parametro (delego 
 * en LabelMapa).
 */
void Mapa::actualizar_data(string id_label, QPixmap& nueva_imagen, 
    int nuevo_tipo, string nuevo_id) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_data(nueva_imagen, nuevo_tipo, nuevo_id);
    }
}

void Mapa::agregar_jugador(string id_label, QPixmap& nueva_imagen) {
    this->jugadores.emplace(id_label, true);
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_imagen(nueva_imagen);
    }
}

/**
 * \brief Metodo virtual de la interfaz ObservadorMapa.
 * 
 * Metodo virtual que es llamado por LabelMapa cuando este es clickeado.
 * Avisa al observador cual LabelMapa fue clickeado.
 */
void Mapa::label_mapa_clickeado(std::string id_label_mapa) {
    this->observador->en_notificacion(id_label_mapa);
}

/**
 * \brief Split de string.
 * 
 * Devuelvo un vector de strings a partir del splitteo de un string y un 
 * caracter delimitador.
 */
vector<string> Mapa::split(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<string> elementos;
    while (std::getline(ss, item, delim)) {
        elementos.emplace_back(item);
    }
    return elementos;
}

/**
 * \brief Getter cantidad de jugadores agregados.
 * 
 * Devuelvo la cantidad de jugadores que fueron agregados al mapa hasta ese
 * momento.
 */
int Mapa::get_cantidad_jugadores_agregados() {
    return this->jugadores.size();
}

void Mapa::cambiar_tamanio(int nueva_cant_filas, int nueva_cant_columnas) {
    if (nueva_cant_columnas == this->columnas && nueva_cant_filas == this->filas) {
        return;
    }

    if (nueva_cant_columnas >= this->columnas && nueva_cant_filas >= this->filas) {
        agrandar_mapa(nueva_cant_filas, nueva_cant_columnas);
    } else if (nueva_cant_columnas <= this->columnas && nueva_cant_filas <= this->filas) {
        achicar_mapa(nueva_cant_filas, nueva_cant_columnas);
    } else if (nueva_cant_columnas < this->columnas && nueva_cant_filas > this->filas) {
        sacar_columnas_agregar_filas(nueva_cant_filas, nueva_cant_columnas);
    } else if (nueva_cant_columnas > this->columnas && nueva_cant_filas < this->filas) {
        sacar_filas_agregar_columnas(nueva_cant_filas, nueva_cant_columnas);
    }


}

void Mapa::agrandar_mapa(int nueva_cant_filas, int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos.
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& data_general = *it;
    
    ++it;
    const json& elem = *it;

    auto it_tiles = elem["sprites"].begin();
    json tile = *it;
    string id = tile["sprites"][0]["id"];
    int tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];
    QPixmap sprite_inicial = generar_sprite_inicial(pos_tiles);
    
    for (int i = 0; i < this->filas; ++i) {
        for (int j = this->columnas; j < nueva_cant_columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, id, tipo,
                pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    for (int j = 0; j < nueva_cant_columnas; ++j) {
        for (int i = this->filas; i < nueva_cant_filas; ++i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, id, tipo,
                pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }


    this->filas = nueva_cant_filas;
    this->columnas = nueva_cant_columnas;
    scroll_area_mapa->setLayout(map_layout);
}

void Mapa::achicar_mapa(int nueva_cant_filas, int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos.
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& data_general = *it;
    
    ++it;
    const json& elem = *it;

    auto it_tiles = elem["sprites"].begin();
    json tile = *it;
    string id = tile["sprites"][0]["id"];
    int tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];
    QPixmap sprite_inicial = generar_sprite_inicial(pos_tiles);
    
    for (int i = 0; i < nueva_cant_filas; ++i) {
        for (int j = (this->columnas - 1); j >= nueva_cant_columnas; --j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
            }

            this->mapa.erase(pos_label);
        }
    }

    for (int j = 0; j < this->columnas; ++j) {
        for (int i = (this->filas - 1); i >= nueva_cant_filas; --i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
            }
            
            this->mapa.erase(pos_label);
        }
    }


    this->filas = nueva_cant_filas;
    this->columnas = nueva_cant_columnas;
    scroll_area_mapa->setLayout(map_layout);
}

void Mapa::sacar_columnas_agregar_filas(int nueva_cant_filas, 
    int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos.
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& data_general = *it;
    
    ++it;
    const json& elem = *it;

    auto it_tiles = elem["sprites"].begin();
    json tile = *it;
    string id = tile["sprites"][0]["id"];
    int tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];
    QPixmap sprite_inicial = generar_sprite_inicial(pos_tiles);
    
    // saco columnas
    for (int i = 0; i < nueva_cant_filas; ++i) {
        for (int j = (this->columnas - 1); j >= nueva_cant_columnas; --j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
            }

            this->mapa.erase(pos_label);
        }
    }

    this->columnas = nueva_cant_columnas;

    // agrego filas
    for (int j = 0; j < nueva_cant_columnas; ++j) {
        for (int i = this->filas; i < nueva_cant_filas; ++i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, id, tipo,
                pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    this->filas = nueva_cant_filas;
    scroll_area_mapa->setLayout(map_layout);
}

void Mapa::sacar_filas_agregar_columnas(int nueva_cant_filas, 
    int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos.
    std::ifstream entrada("../sprites/terrain/terrenos.json");
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& data_general = *it;
    
    ++it;
    const json& elem = *it;

    auto it_tiles = elem["sprites"].begin();
    json tile = *it;
    string id = tile["sprites"][0]["id"];
    int tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];
    QPixmap sprite_inicial = generar_sprite_inicial(pos_tiles);

    // saco filas
    for (int j = 0; j < this->columnas; ++j) {
        for (int i = (this->filas - 1); i >= nueva_cant_filas; --i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
            }
            
            this->mapa.erase(pos_label);
        }
    }

    this->filas = nueva_cant_filas;

    // agrego columnas
    for (int i = 0; i < this->filas; ++i) {
        for (int j = this->columnas; j < nueva_cant_columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, id, tipo,
                pos_label, this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    this->columnas = nueva_cant_columnas;
    scroll_area_mapa->setLayout(map_layout);
}

/**
 * \brief Destructor de Mapa.
 * 
 * Libero los LabelMapa del heap (fueron tomados los recursos en 
 * Mapa::inicializar_mapa o en Mapa::parsear_json).
 */
Mapa::~Mapa() {
    map<string, LabelMapa*>::iterator it = this->mapa.begin();
    for (; it != this->mapa.end(); ++it) {
        delete it->second;
    }
}
        