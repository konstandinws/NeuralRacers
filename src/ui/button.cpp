#include "ui/button.hpp"

namespace ui {

Button::Button(ButtonProps props) : props(std::move(props)) {
    bounds.width = this->props.size.x;
    bounds.height = this->props.size.y;
}

void Button::update(const Input& input) {
    hovered = CheckCollisionPointRec(input.mouse, bounds);

    if (hovered && input.mousePressed) {
        held = true;
    }

    if (input.mouseReleased) {
        if (held && hovered && props.onClick) {
            props.onClick();
        }
        held = false;
    }
}

void Button::draw() const {
    const ButtonStyle& style = props.style;

    Color background = style.background;
    if (held && hovered) {
        background = style.pressed;
    } else if (hovered) {
        background = style.hover;
    }

    DrawRectangleRounded(bounds, style.roundness, 8, background);
    DrawRectangleRoundedLinesEx(bounds, style.roundness, 8, 2.0f, hovered ? style.hoverBorder : style.border);

    // texte centre dans le bouton
    int textWidth = MeasureText(props.text.c_str(), style.fontSize);
    DrawText(props.text.c_str(),
             (int)(bounds.x + (bounds.width - textWidth) / 2.0f),
             (int)(bounds.y + (bounds.height - style.fontSize) / 2.0f),
             style.fontSize, style.text);
}

}
