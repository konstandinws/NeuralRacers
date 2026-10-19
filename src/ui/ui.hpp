#pragma once
#include <vector>
#include "ui/component.hpp"
#include "ui/button.hpp"
#include "ui/label.hpp"
#include "ui/column.hpp"
#include "ui/scrollView.hpp"

// fonctions de creation pour decrire l'interface comme un arbre, a la maniere du JSX :
//
//   ui::column({.spacing = 20},
//       ui::label({.text = "Titre", .fontSize = 40}),
//       ui::button({.text = "OK", .onClick = [] { ... }})
//   );
namespace ui {

inline ComponentPtr button(ButtonProps props) {
    return std::make_unique<Button>(std::move(props));
}

inline ComponentPtr label(LabelProps props) {
    return std::make_unique<Label>(std::move(props));
}

template <typename... Children>
ComponentPtr column(ColumnProps props, Children... children) {
    std::vector<ComponentPtr> list;
    (list.push_back(std::move(children)), ...);
    return std::make_unique<Column>(props, std::move(list));
}

// variante pour une liste construite dynamiquement (ex : un bouton par fichier)
inline ComponentPtr columnOf(ColumnProps props, std::vector<ComponentPtr> children) {
    return std::make_unique<Column>(props, std::move(children));
}

inline ComponentPtr scrollView(ScrollViewProps props, ComponentPtr content) {
    return std::make_unique<ScrollView>(props, std::move(content));
}

}
