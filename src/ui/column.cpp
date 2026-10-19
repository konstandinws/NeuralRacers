#include "ui/column.hpp"
#include <algorithm>

namespace ui {

Column::Column(ColumnProps props, std::vector<ComponentPtr> children) : props(props), children(std::move(children)) {
    // la taille de la colonne depend de celle de ses enfants
    for (const ComponentPtr& child : this->children) {
        Rectangle childBounds = child->getBounds();
        bounds.width = std::max(bounds.width, childBounds.width);
        bounds.height += childBounds.height;
    }
    if (!this->children.empty()) {
        bounds.height += props.spacing * (this->children.size() - 1);
    }
    layout();
}

void Column::layout() {
    float y = bounds.y;
    for (ComponentPtr& child : children) {
        Rectangle childBounds = child->getBounds();
        child->setPosition({bounds.x + (bounds.width - childBounds.width) / 2.0f, y});
        y += childBounds.height + props.spacing;
    }
}

void Column::setPosition(Vector2 position) {
    Component::setPosition(position);
    layout();
}

void Column::update(const Input& input) {
    for (ComponentPtr& child : children) {
        child->update(input);
    }
}

void Column::draw() const {
    for (const ComponentPtr& child : children) {
        child->draw();
    }
}

}
