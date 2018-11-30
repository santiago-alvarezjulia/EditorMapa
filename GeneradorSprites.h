#ifndef GENERADOR_SPRITES_H
#define GENERADOR_SPRITES_H

#include <string>
#include <map>
#include <QPixmap>
#include <vector>
#include <fstream>

struct Sprite {
    int tipo;
    QPixmap sprite;
};

class GeneradorSprites {
    public:
        GeneradorSprites();
    
        std::map<std::string, Sprite> generar_sprites_posibles();

        QPixmap generar_sprite_inicial(std::vector<uint32_t>& pos_tiles);

        ~GeneradorSprites() {};

    private:
        QPixmap imagen_terrenos;
    
};

#endif // GENERADOR_SPRITES_H
