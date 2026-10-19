#pragma once
#include <functional>
#include <string>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct ButtonStyle {
    Color background  = theme::surface;
    Color hover       = theme::surfaceHover;
    Color pressed     = theme::surfaceDark;
    Color border      = theme::border;
    Color hoverBorder = theme::accent;
    Color text        = theme::text;
    int fontSize      = 20;
    float roundness   = 0.3f;
};

struct ButtonProps {
    std::string text;
    std::function<void()> onClick;
    Vector2 size = {200, 50};
    ButtonStyle style = {};
};

// bouton cliquable : onClick est appele quand on relache le clic sur le bouton
// (comme un vrai bouton : on peut annuler en sortant du bouton avant de relacher)
class Button : public Component {
public:
    explicit Button(ButtonProps props);

    void update(const Input& input) override;
    void draw() const override;

private:
    ButtonProps props;
    bool hovered = false;
    bool held = false;   // le clic a commence sur ce bouton
};

}
