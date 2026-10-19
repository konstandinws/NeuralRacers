#pragma once
#include <vector>
#include "ui/component.hpp"

namespace ui {

enum class Align { Center, Start };

struct StackProps {
    float spacing = 10.0f;        // espace entre deux enfants
    Align align = Align::Center;  // placement sur l'autre axe : centre, ou en haut / a gauche
    float minWidth = 0.0f;        // evite que la mise en page bouge quand le contenu change de largeur
};

using ColumnProps = StackProps;
using RowProps = StackProps;

// conteneur qui aligne ses enfants les uns apres les autres et les centre sur l'autre axe
// (equivalent d'un flexbox : flex-direction column ou row, align-items center)
class Stack : public Component {
public:
    enum class Direction { Vertical, Horizontal };

    Stack(Direction direction, StackProps props, std::vector<ComponentPtr> children);

    void update(const Input& input) override;
    void draw() const override;
    void setPosition(Vector2 position) override;

private:
    void layout();

    Direction direction;
    StackProps props;
    std::vector<ComponentPtr> children;
};

}
