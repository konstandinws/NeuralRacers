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
    bool autoFocus = false;                           // actif des sa creation
    std::function<void()> onBlur;                     // appele quand le champ perd le focus
    bool centerText = false;
};

// champ de saisie sur une ligne
// un clic dessus lui donne le focus, un clic ailleurs le lui retire : seul le champ actif recoit le clavier
class TextInput : public Component {
public:
    explicit TextInput(TextInputProps props);

    void update(const Input& input) override;
    void draw() const override;

    // pour un composant parent qui doit changer le texte (ex : Stepper apres un clic sur +1)
    void setText(const std::string& newText) { text = newText; }
    const std::string& getText() const { return text; }

private:
    TextInputProps props;
    std::string text;
    bool focused = false;
};

}
