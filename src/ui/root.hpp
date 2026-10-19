#pragma once
#include <functional>
#include "ui/component.hpp"

namespace ui {

// racine d'un arbre de composants qui se reconstruit a partir de l'etat de l'ecran, comme le rendu React :
//
//   root.mount([this] { return buildUI(); });   // une fois
//   root.invalidate();                          // dans un callback, apres avoir change l'etat
//
// la reconstruction a lieu au debut du prochain update, jamais pendant un callback :
// un composant ne peut donc pas etre detruit pendant qu'il execute son propre onClick
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
