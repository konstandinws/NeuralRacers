#pragma once
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct ModalProps {
    float padding = 30.0f;
    Color overlay = {0, 0, 0, 170};  // overlay on the rest of the screen
    Color background = theme::surface;
    Color border = theme::border;
};

// popup centered on screen: darkens everything else and shows its content in a panel
// it covers the whole screen, so nothing below receives clicks while it is open
class Modal : public Component {
public:
    Modal(ModalProps props, ComponentPtr content);

    void update(const Input& input) override;
    void draw() const override;

private:
    Rectangle panel() const;

    ModalProps props;
    ComponentPtr content;
};

}
