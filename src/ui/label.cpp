#include "ui/label.hpp"

namespace ui {

Label::Label(LabelProps props) : props(std::move(props)) {
    bounds.width = (float)MeasureText(this->props.text.c_str(), this->props.fontSize);
    bounds.height = (float)this->props.fontSize;
}

void Label::draw() const {
    DrawText(props.text.c_str(), (int)bounds.x, (int)bounds.y, props.fontSize, props.color);
}

}
