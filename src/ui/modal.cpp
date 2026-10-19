#include "ui/modal.hpp"

namespace ui {

Modal::Modal(ModalProps props, ComponentPtr content) : props(props), content(std::move(content)) {
    bounds = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    placeCentered(*this->content, bounds.width / 2.0f, (bounds.height - this->content->getBounds().height) / 2.0f);
}

Rectangle Modal::panel() const {
    Rectangle inner = content->getBounds();
    return {inner.x - props.padding, inner.y - props.padding, inner.width + 2 * props.padding, inner.height + 2 * props.padding};
}

void Modal::update(const Input& input) {
    content->update(input);
}

void Modal::draw() const {
    DrawRectangleRec(bounds, props.overlay);

    Rectangle rect = panel();
    DrawRectangleRounded(rect, 0.08f, 8, props.background);
    DrawRectangleRoundedLinesEx(rect, 0.08f, 8, 2.0f, props.border);

    content->draw();
}

}
