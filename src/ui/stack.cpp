#include "ui/stack.hpp"
#include <algorithm>

namespace ui {

Stack::Stack(Direction direction, StackProps props, std::vector<ComponentPtr> children)
    : direction(direction), props(props), children(std::move(children)) {
    // la taille du conteneur depend de celle de ses enfants :
    // somme sur l'axe principal, maximum sur l'autre axe
    bool vertical = direction == Direction::Vertical;
    for (const ComponentPtr& child : this->children) {
        Rectangle childBounds = child->getBounds();
        if (vertical) {
            bounds.width = std::max(bounds.width, childBounds.width);
            bounds.height += childBounds.height;
        } else {
            bounds.width += childBounds.width;
            bounds.height = std::max(bounds.height, childBounds.height);
        }
    }
    if (!this->children.empty()) {
        float gaps = props.spacing * (this->children.size() - 1);
        (vertical ? bounds.height : bounds.width) += gaps;
    }
    bounds.width = std::max(bounds.width, props.minWidth);
    layout();
}

void Stack::layout() {
    float cursor = (direction == Direction::Vertical) ? bounds.y : bounds.x;
    float centered = (props.align == Align::Center) ? 0.5f : 0.0f;
    for (ComponentPtr& child : children) {
        Rectangle childBounds = child->getBounds();
        if (direction == Direction::Vertical) {
            child->setPosition({bounds.x + (bounds.width - childBounds.width) * centered, cursor});
            cursor += childBounds.height + props.spacing;
        } else {
            child->setPosition({cursor, bounds.y + (bounds.height - childBounds.height) * centered});
            cursor += childBounds.width + props.spacing;
        }
    }
}

void Stack::setPosition(Vector2 position) {
    Component::setPosition(position);
    layout();
}

void Stack::update(const Input& input) {
    for (ComponentPtr& child : children) {
        child->update(input);
    }
}

void Stack::draw() const {
    for (const ComponentPtr& child : children) {
        child->draw();
    }
}

}
