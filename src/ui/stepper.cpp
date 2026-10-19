#include "ui/stepper.hpp"
#include "ui/theme.hpp"
#include <algorithm>
#include <string>

namespace ui {

static constexpr float BUTTON_SIZE = 50.0f;
static constexpr int FONT_SIZE = 30;

Stepper::Stepper(StepperProps props)
    : props(std::move(props)),
      value(std::clamp(this->props.value, this->props.min, this->props.max)),
      minus({.text = "-", .onClick = [this] { change(-this->props.step); }, .size = {BUTTON_SIZE, BUTTON_SIZE}}),
      plus({.text = "+", .onClick = [this] { change(this->props.step); }, .size = {BUTTON_SIZE, BUTTON_SIZE}}) {
    bounds.width = this->props.width;
    bounds.height = BUTTON_SIZE;
    setPosition({0, 0});
}

void Stepper::change(int delta) {
    int newValue = std::clamp(value + delta, props.min, props.max);
    if (newValue != value) {
        value = newValue;
        if (props.onChange) {
            props.onChange(value);
        }
    }
}

void Stepper::setPosition(Vector2 position) {
    Component::setPosition(position);
    minus.setPosition({bounds.x, bounds.y});
    plus.setPosition({bounds.x + bounds.width - BUTTON_SIZE, bounds.y});
}

void Stepper::update(const Input& input) {
    minus.update(input);
    plus.update(input);
}

void Stepper::draw() const {
    minus.draw();
    plus.draw();

    std::string text = std::to_string(value);
    int textWidth = MeasureText(text.c_str(), FONT_SIZE);
    DrawText(text.c_str(),
             (int)(bounds.x + (bounds.width - textWidth) / 2.0f),
             (int)(bounds.y + (bounds.height - FONT_SIZE) / 2.0f),
             FONT_SIZE, theme::text);
}

}
