#pragma once
#include <functional>
#include <string>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct ButtonStyle {
    Color background  = theme::surface;
    Color hover       = theme::surfaceHover;
    Color pressed     = theme::surfaceDark;
    Color border      = theme::border;
    Color hoverBorder = theme::accent;
    Color text        = theme::text;
    int fontSize      = 20;
    float roundness   = 0.3f;
};

struct ButtonProps {
    std::string text;
    std::function<void()> onClick;
    Vector2 size = {200, 50};
    bool selected = false;   // highlighted (e.g. item chosen in a list)
    bool disabled = false;   // grayed out and not clickable
    ButtonStyle style = {};
};

// clickable button: onClick is called when the click is released on the button
// (like a real button: you can cancel by leaving the button before releasing)
class Button : public Component {
public:
    explicit Button(ButtonProps props);

    void update(const Input& input) override;
    void draw() const override;

private:
    ButtonProps props;
    bool hovered = false;
    bool held = false;   // the click started on this button
};

}
