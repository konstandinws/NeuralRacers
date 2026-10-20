#include "ui/textInput.hpp"
#include <cmath>

namespace ui {

TextInput::TextInput(TextInputProps props) : props(std::move(props)) {
    text = this->props.value;
    focused = this->props.autoFocus;
    bounds.width = this->props.size.x;
    bounds.height = this->props.size.y;
}

void TextInput::update(const Input& input) {
    // a click gives or removes focus depending on whether it is on the field
    if (input.mousePressed) {
        bool wasFocused = focused;
        focused = CheckCollisionPointRec(input.mouse, bounds);
        if (wasFocused && !focused && props.onBlur) {
            props.onBlur();
        }
    }

    if (!focused) {
        return;
    }

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
    DrawRectangleLinesEx(bounds, 2.0f, focused ? theme::accent : theme::border);

    float padding = 12.0f;
    int textWidth = MeasureText(text.c_str(), props.fontSize);
    int textX = props.centerText ? (int)(bounds.x + (bounds.width - textWidth) / 2.0f) : (int)(bounds.x + padding);
    int textY = (int)(bounds.y + (bounds.height - props.fontSize) / 2.0f);

    if (text.empty()) {
        // offset so it is not hidden by the cursor
        DrawText(props.placeholder.c_str(), textX + 6, textY, props.fontSize, theme::border);
    } else {
        DrawText(text.c_str(), textX, textY, props.fontSize, theme::text);
    }

    // blinking cursor after the text, only on the focused field
    if (focused && std::fmod(GetTime(), 1.0) < 0.5) {
        DrawRectangle(textX + textWidth + 2, textY, 2, props.fontSize, theme::text);
    }
}

}
