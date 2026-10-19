#pragma once
#include <functional>
#include "ui/component.hpp"
#include "ui/button.hpp"

namespace ui {

struct StepperProps {
    int value = 0;
    int min = 0;
    int max = 100;
    int step = 1;
    std::function<void(int)> onChange;
    float width = 260.0f;
};

// choix d'un nombre entier :  [-]  valeur  [+]
class Stepper : public Component {
public:
    explicit Stepper(StepperProps props);

    // les boutons capturent this : le stepper ne doit pas etre copie ou deplace
    Stepper(const Stepper&) = delete;
    Stepper& operator=(const Stepper&) = delete;

    void update(const Input& input) override;
    void draw() const override;
    void setPosition(Vector2 position) override;

private:
    void change(int delta);

    StepperProps props;
    int value;
    Button minus;
    Button plus;
};

}
