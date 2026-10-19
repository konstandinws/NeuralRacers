#pragma once
#include <functional>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct ScrollViewProps {
    Vector2 size = {400, 300};
    float padding = 10.0f;
    float scrollSpeed = 40.0f;  // pixels par cran de molette
    Color background = theme::surfaceDark;
    Color border = theme::border;
    // defilement initial + callback : permet au parent de garder la position quand l'interface est reconstruite
    float offset = 0.0f;
    std::function<void(float)> onScroll;
};

// zone de taille fixe qui affiche une partie de son contenu et defile a la molette
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
    float offset = 0.0f;  // de combien de pixels le contenu est remonte
};

}
