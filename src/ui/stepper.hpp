#pragma once
#include <functional>
#include <vector>
#include "ui/component.hpp"
#include "ui/button.hpp"
#include "ui/textInput.hpp"

namespace ui {

struct StepperProps {
    int value = 0;
    int min = 0;
    int max = 100;
    // one - button and one + button per step, largest to smallest: {10, 1} -> [-10] [-1] value [+1] [+10]
    std::vector<int> steps = {1};
    std::function<void(int)> onChange;
};

// picks an integer with buttons, or by typing the value in the middle field
// the typed value is committed with Enter or by clicking elsewhere, then clamped between min and max
class Stepper : public Component {
public:
    explicit Stepper(StepperProps props);

    // the buttons and the field capture this: the stepper must not be copied or moved
    Stepper(const Stepper&) = delete;
    Stepper& operator=(const Stepper&) = delete;

    void update(const Input& input) override;
    void draw() const override;
    void setPosition(Vector2 position) override;

private:
    void setValue(int newValue);
    void commitTypedValue();

    StepperProps props;
    int value;
    std::vector<Button> minusButtons;  // [-10] [-1]
    std::vector<Button> plusButtons;   // [+1] [+10]
    TextInput field;
};

}
