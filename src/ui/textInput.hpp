#pragma once
#include <functional>
#include <string>
#include "ui/component.hpp"
#include "ui/theme.hpp"

namespace ui {

struct TextInputProps {
    std::string value;                                // texte initial
    std::string placeholder;                          // affiche en gris quand le champ est vide
    std::function<void(const std::string&)> onChange; // appele a chaque modification
    std::function<void()> onSubmit;                   // appele quand on appuie sur Entree
    // transforme chaque caractere tape : renvoie le caractere a inserer, ou 0 pour le refuser
    // sans filtre, seuls les caracteres ASCII imprimables sont acceptes
    std::function<int(int)> filter;
    int maxLength = 28;
    Vector2 size = {360, 44};
    int fontSize = 20;
};

// champ de saisie sur une ligne, toujours actif (il n'y a qu'un champ par ecran pour l'instant)
class TextInput : public Component {
public:
    explicit TextInput(TextInputProps props);

    void update(const Input& input) override;
    void draw() const override;

private:
    TextInputProps props;
    std::string text;
};

}
