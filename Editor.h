#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "Tabs.h"
#include "Mapa.h"

class Editor : public QWidget {
    public:
        explicit Editor(QWidget *parent = 0);
        ~Editor();
    private:
        Mapa* mapa;
        Tabs* tabs;
};

#endif // EDITOR_H