#include "Editor.h"
// Cargo el archivo generado por uic, leer el CMakelist.txt para mas info
#include "ui_Editor.h"
using std::string;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion
    editor.setupUi(this);
    
    this->tabs = new Tabs(this);
    this->mapa = new Mapa(this);
    this->mapa->agregar_observador(this);
}

void Editor::en_notificacion(string id_label) {
    if (this->tabs->get_id_label_clickeado() != "") {
        this->mapa->actualizar_imagen(id_label);
    } 
}

Editor::~Editor() {
    delete this->tabs;
    delete this->mapa;
}