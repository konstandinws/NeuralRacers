#pragma once
#include <string>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct LabelProps {
    std::string text;
    int fontSize = 20;
    Color color = theme::text;
};

// plain text, its size is computed from the text
class Label : public Component {
public:
    explicit Label(LabelProps props);

    void draw() const override;

private:
    LabelProps props;
};

}
