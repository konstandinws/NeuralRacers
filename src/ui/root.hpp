#pragma once
#include <functional>
#include "ui/component.hpp"

namespace ui {

// root of a component tree rebuilt from the screen's state, like React rendering:
//
//   root.mount([this] { return buildUI(); });   // once
//   root.invalidate();                          // in a callback, after changing the state
//
// the rebuild happens at the start of the next update, never during a callback:
// so a component can never be destroyed while running its own onClick
class Root {
public:
    void mount(std::function<ComponentPtr()> render) {
        this->render = std::move(render);
        dirty = true;
    }

    void invalidate() { dirty = true; }

    void update(const Input& input) {
        if (dirty && render) {
            tree = render();
            dirty = false;
        }
        if (tree) {
            tree->update(input);
        }
    }

    void draw() const {
        if (tree) {
            tree->draw();
        }
    }

private:
    std::function<ComponentPtr()> render;
    ComponentPtr tree;
    bool dirty = false;
};

}
