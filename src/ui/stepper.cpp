#include "ui/stepper.hpp"
#include <algorithm>
#include <string>

namespace ui {

static constexpr float BUTTON_SIZE = 50.0f;
static constexpr float FIELD_WIDTH = 80.0f;
static constexpr float GAP = 6.0f;

static int digitsOnly(int c) {
    return (c >= '0' && c <= '9') ? c : 0;
}

Stepper::Stepper(StepperProps props)
    : props(std::move(props)),
      value(std::clamp(this->props.value, this->props.min, this->props.max)),
      field({
          .value = std::to_string(value),
          .onSubmit = [this] { commitTypedValue(); },
          .filter = digitsOnly,
          .maxLength = (int)std::to_string(this->props.max).size(),
          .size = {FIELD_WIDTH, BUTTON_SIZE},
          .fontSize = 26,
          .onBlur = [this] { commitTypedValue(); },
          .centerText = true,
      }) {
    for (int step : this->props.steps) {
        std::string amount = std::to_string(step);
        minusButtons.push_back(Button({.text = "-" + amount, .onClick = [this, step] { setValue(value - step); }, .size = {BUTTON_SIZE, BUTTON_SIZE}}));
    }
    // + side: smallest to largest so the row is symmetric
    for (auto it = this->props.steps.rbegin(); it != this->props.steps.rend(); ++it) {
        int step = *it;
        plusButtons.push_back(Button({.text = "+" + std::to_string(step), .onClick = [this, step] { setValue(value + step); }, .size = {BUTTON_SIZE, BUTTON_SIZE}}));
    }

    int buttonCount = (int)(minusButtons.size() + plusButtons.size());
    bounds.width = buttonCount * (BUTTON_SIZE + GAP) + FIELD_WIDTH;
    bounds.height = BUTTON_SIZE;
    setPosition({0, 0});
}

void Stepper::setValue(int newValue) {
    newValue = std::clamp(newValue, props.min, props.max);
    field.setText(std::to_string(newValue));  // the field always shows the accepted value
    if (newValue != value) {
        value = newValue;
        if (props.onChange) {
            props.onChange(value);
        }
    }
}

void Stepper::commitTypedValue() {
    const std::string& typed = field.getText();
    // empty field: restore the previous value
    setValue(typed.empty() ? value : std::stoi(typed));
}

void Stepper::setPosition(Vector2 position) {
    Component::setPosition(position);

    float x = bounds.x;
    for (Button& button : minusButtons) {
        button.setPosition({x, bounds.y});
        x += BUTTON_SIZE + GAP;
    }
    field.setPosition({x, bounds.y});
    x += FIELD_WIDTH + GAP;
    for (Button& button : plusButtons) {
        button.setPosition({x, bounds.y});
        x += BUTTON_SIZE + GAP;
    }
}

void Stepper::update(const Input& input) {
    // field first: clicking a button commits the pending input before the button applies
    field.update(input);
    for (Button& button : minusButtons) {
        button.update(input);
    }
    for (Button& button : plusButtons) {
        button.update(input);
    }
}

void Stepper::draw() const {
    for (const Button& button : minusButtons) {
        button.draw();
    }
    field.draw();
    for (const Button& button : plusButtons) {
        button.draw();
    }
}

}
