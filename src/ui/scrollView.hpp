#pragma once
#include <functional>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct ScrollViewProps {
    Vector2 size = {400, 300};
    float padding = 10.0f;
    float scrollSpeed = 40.0f;  // pixels per mouse wheel notch
    Color background = theme::surfaceDark;
    Color border = theme::border;
    // initial scroll + callback: lets the parent keep the position when the UI is rebuilt
    float offset = 0.0f;
    std::function<void(float)> onScroll;
};

// fixed-size area that shows part of its content and scrolls with the mouse wheel
class ScrollView : public Component {
public:
    ScrollView(ScrollViewProps props, ComponentPtr content);

    void update(const Input& input) override;
    void draw() const override;
    void setPosition(Vector2 position) override;

    static constexpr float SCROLLBAR_WIDTH = 6.0f;

private:
    float maxOffset() const;
    void layoutContent();

    ScrollViewProps props;
    ComponentPtr content;
    float offset = 0.0f;  // how many pixels the content is scrolled up
};

}
