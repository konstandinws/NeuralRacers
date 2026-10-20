#pragma once
#include <memory>
#include "raylib.h"
#include "ui/input.hpp"

namespace ui {

// basic UI building block, like a React component:
// it receives its "props" on construction, handles input in update and draws itself in draw
class Component {
public:
    virtual ~Component() = default;

    virtual void update(const Input& input) { (void)input; }
    virtual void draw() const = 0;

    // sets the top-left corner, containers override it to move their children
    virtual void setPosition(Vector2 position) {
        bounds.x = position.x;
        bounds.y = position.y;
    }

    Rectangle getBounds() const { return bounds; }

protected:
    Rectangle bounds = {0, 0, 0, 0};
};

using ComponentPtr = std::unique_ptr<Component>;

// centers a component horizontally on centerX, with its top at y
inline void placeCentered(Component& component, float centerX, float y) {
    component.setPosition({centerX - component.getBounds().width / 2.0f, y});
}

}
