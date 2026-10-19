#pragma once
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct ModalProps {
    float padding = 30.0f;
    Color overlay = {0, 0, 0, 170};  // voile sur le reste de l'ecran
    Color background = theme::surface;
    Color border = theme::border;
};

// popup centre a l'ecran : assombrit tout le reste et affiche son contenu dans un panneau
// il couvre tout l'ecran, donc rien en dessous ne recoit de clics tant qu'il est ouvert
class Modal : public Component {
public:
    Modal(ModalProps props, ComponentPtr content);

    void update(const Input& input) override;
    void draw() const override;

private:
    Rectangle panel() const;

    ModalProps props;
    ComponentPtr content;
};

}
