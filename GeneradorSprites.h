#ifndef GENERADOR_SPRITES_H
#define GENERADOR_SPRITES_H

#include <string>
#include <map>
#include <QPixmap>
#include <vector>
#include <fstream>

struct Sprite {
    std::string id;
    int tipo;
    QPixmap imagen;
};

class GeneradorSprites {
    public:
        GeneradorSprites();
    
        std::map<std::string, Sprite> generar_sprites_posibles();

        Sprite generar_sprite_inicial();

        Sprite generar_sprite(std::string id, int tipo, 
            std::vector<uint32_t> pos_tiles);

        ~GeneradorSprites() {};

    private:
        QPixmap imagen_terrenos;
    
};

#endif // GENERADOR_SPRITES_H
