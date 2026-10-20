#pragma once
#include <vector>
#include "ui/component.hpp"

namespace ui {

enum class Align { Center, Start };

struct StackProps {
    float spacing = 10.0f;        // space between two children
    Align align = Align::Center;  // placement on the other axis: centered, or top / left
    float minWidth = 0.0f;        // keeps the layout from shifting when the content width changes
};

using ColumnProps = StackProps;
using RowProps = StackProps;

// container that lays out its children one after another and centers them on the other axis
// (like a flexbox: flex-direction column or row, align-items center)
class Stack : public Component {
public:
    enum class Direction { Vertical, Horizontal };

    Stack(Direction direction, StackProps props, std::vector<ComponentPtr> children);

    void update(const Input& input) override;
    void draw() const override;
    void setPosition(Vector2 position) override;

private:
    void layout();

    Direction direction;
    StackProps props;
    std::vector<ComponentPtr> children;
};

}
