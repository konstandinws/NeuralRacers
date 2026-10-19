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
    // un bouton - et un bouton + par pas, du plus grand au plus petit : {10, 1} -> [-10] [-1] valeur [+1] [+10]
    std::vector<int> steps = {1};
    std::function<void(int)> onChange;
};

// choix d'un nombre entier avec des boutons, ou en tapant la valeur dans le champ du milieu
// la valeur tapee est validee avec Entree ou en cliquant ailleurs, puis ramenee entre min et max
class Stepper : public Component {
public:
    explicit Stepper(StepperProps props);

    // les boutons et le champ capturent this : le stepper ne doit pas etre copie ou deplace
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
