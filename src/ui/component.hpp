#pragma once
#include <memory>
#include "raylib.h"
#include "ui/input.hpp"

namespace ui {

// brique de base de l'interface, comme un composant React :
// il recoit ses "props" a la construction, gere les entrees dans update et se dessine dans draw
class Component {
public:
    virtual ~Component() = default;

    virtual void update(const Input& input) { (void)input; }
    virtual void draw() const = 0;

    // place le coin haut-gauche, les conteneurs la redefinissent pour deplacer leurs enfants
    virtual void setPosition(Vector2 position) {
        bounds.x = position.x;
        bounds.y = position.y;
    }

    Rectangle getBounds() const { return bounds; }

protected:
    Rectangle bounds = {0, 0, 0, 0};
};

using ComponentPtr = std::unique_ptr<Component>;

// centre un composant horizontalement sur centerX, avec son haut en y
inline void placeCentered(Component& component, float centerX, float y) {
    component.setPosition({centerX - component.getBounds().width / 2.0f, y});
}

}
