#pragma once
#include <functional>
#include <string>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct TextInputProps {
    std::string value;                                // initial text
    std::string placeholder;                          // shown in gray when the field is empty
    std::function<void(const std::string&)> onChange; // called on every change
    std::function<void()> onSubmit;                   // called when Enter is pressed
    // transforms each typed character: returns the character to insert, or 0 to reject it
    // without a filter, only printable ASCII characters are accepted
    std::function<int(int)> filter;
    int maxLength = 28;
    Vector2 size = {360, 44};
    int fontSize = 20;
    bool autoFocus = false;                           // focused as soon as it is created
    std::function<void()> onBlur;                     // called when the field loses focus
    bool centerText = false;
};

// single-line text field
// clicking it gives it focus, clicking elsewhere removes it: only the focused field receives keyboard input
class TextInput : public Component {
public:
    explicit TextInput(TextInputProps props);

    void update(const Input& input) override;
    void draw() const override;

    // for a parent component that needs to change the text (e.g. Stepper after a click on +1)
    void setText(const std::string& newText) { text = newText; }
    const std::string& getText() const { return text; }

private:
    TextInputProps props;
    std::string text;
    bool focused = false;
};

}
