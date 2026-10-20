#include "ui/scrollView.hpp"
#include "raymath.h"
#include <algorithm>

namespace ui {

ScrollView::ScrollView(ScrollViewProps props, ComponentPtr content) : props(std::move(props)), content(std::move(content)) {
    bounds.width = this->props.size.x;
    bounds.height = this->props.size.y;
    offset = Clamp(this->props.offset, 0.0f, maxOffset());
    layoutContent();
}

float ScrollView::maxOffset() const {
    float contentHeight = content->getBounds().height + 2.0f * props.padding;
    return std::max(0.0f, contentHeight - bounds.height);
}

void ScrollView::layoutContent() {
    // content centered horizontally, shifted up by the scroll offset
    float contentWidth = content->getBounds().width;
    content->setPosition({bounds.x + (bounds.width - contentWidth) / 2.0f, bounds.y + props.padding - offset});
}

void ScrollView::setPosition(Vector2 position) {
    Component::setPosition(position);
    layoutContent();
}

void ScrollView::update(const Input& input) {
    bool mouseInside = CheckCollisionPointRec(input.mouse, bounds);

    if (mouseInside && input.wheel != 0.0f) {
        offset = Clamp(offset - input.wheel * props.scrollSpeed, 0.0f, maxOffset());
        layoutContent();
        if (props.onScroll) {
            props.onScroll(offset);
        }
    }

    // items hidden outside the visible area must not be clickable
    content->update(mouseInside ? input : input.withoutMouse());
}

void ScrollView::draw() const {
    DrawRectangleRec(bounds, props.background);

    // only draw what is inside the visible area
    BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
    content->draw();
    EndScissorMode();

    DrawRectangleLinesEx(bounds, 2.0f, props.border);

    // scrollbar, only if the content overflows
    float max = maxOffset();
    if (max > 0.0f) {
        float thumbHeight = bounds.height * bounds.height / (bounds.height + max);
        float thumbY = bounds.y + (bounds.height - thumbHeight) * (offset / max);
        DrawRectangleRec({bounds.x + bounds.width - SCROLLBAR_WIDTH - 2.0f, thumbY, SCROLLBAR_WIDTH, thumbHeight}, props.border);
    }
}

}
