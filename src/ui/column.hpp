#pragma once
#include <vector>
#include "ui/component.hpp"

namespace ui {

struct ColumnProps {
    float spacing = 10.0f;  // espace vertical entre deux enfants
};

// conteneur qui empile ses enfants verticalement, centres horizontalement
// (equivalent d'un flex-direction: column; align-items: center)
class Column : public Component {
public:
    Column(ColumnProps props, std::vector<ComponentPtr> children);

    void update(const Input& input) override;
    void draw() const override;
    void setPosition(Vector2 position) override;

private:
    void layout();

    ColumnProps props;
    std::vector<ComponentPtr> children;
};

}
