#include "ui/textInput.hpp"
#include <cmath>

namespace ui {

TextInput::TextInput(TextInputProps props) : props(std::move(props)) {
    text = this->props.value;
    bounds.width = this->props.size.x;
    bounds.height = this->props.size.y;
}

void TextInput::update(const Input& input) {
    bool changed = false;

    for (int c : input.typed) {
        int accepted = props.filter ? props.filter(c) : ((c >= 32 && c < 127) ? c : 0);
        if (accepted != 0 && (int)text.size() < props.maxLength) {
            text.push_back((char)accepted);
            changed = true;
        }
    }

    if (input.backspace && !text.empty()) {
        text.pop_back();
        changed = true;
    }

    if (changed && props.onChange) {
        props.onChange(text);
    }

    if (input.enter && props.onSubmit) {
        props.onSubmit();
    }
}

void TextInput::draw() const {
    DrawRectangleRec(bounds, theme::surfaceDark);
    DrawRectangleLinesEx(bounds, 2.0f, theme::accent);

    float padding = 12.0f;
    int textX = (int)(bounds.x + padding);
    int textY = (int)(bounds.y + (bounds.height - props.fontSize) / 2.0f);

    if (text.empty()) {
        // decale pour ne pas etre cache par le curseur
        DrawText(props.placeholder.c_str(), textX + 6, textY, props.fontSize, theme::border);
    } else {
        DrawText(text.c_str(), textX, textY, props.fontSize, theme::text);
    }

    // curseur qui clignote apres le texte
    if (std::fmod(GetTime(), 1.0) < 0.5) {
        int caretX = textX + MeasureText(text.c_str(), props.fontSize) + 2;
        DrawRectangle(caretX, textY, 2, props.fontSize, theme::text);
    }
}

}
