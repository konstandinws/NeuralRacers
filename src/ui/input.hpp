#pragma once
#include <vector>
#include "raylib.h"

namespace ui {

// etat de la souris et du clavier pour une frame, lu une seule fois puis transmis a tout l'arbre de composants
// poll() doit etre appele a chaque frame : il vide la file des caracteres tapes de raylib
struct Input {
    Vector2 mouse = {0, 0};
    bool mousePressed = false;   // bouton gauche enfonce pendant cette frame
    bool mouseReleased = false;  // bouton gauche relache pendant cette frame
    float wheel = 0.0f;

    std::vector<int> typed;      // caracteres tapes pendant cette frame (codepoints)
    bool backspace = false;      // appui ou repetition (touche maintenue)
    bool enter = false;

    static Input poll() {
        Input input;
        input.mouse = GetMousePosition();
        input.mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        input.mouseReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
        input.wheel = GetMouseWheelMove();

        for (int c = GetCharPressed(); c != 0; c = GetCharPressed()) {
            input.typed.push_back(c);
        }
        input.backspace = IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE);
        input.enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
        return input;
    }

    // copie sans la souris, pour les zones masquees (ex : boutons hors de la partie visible d'un ScrollView)
    // mouseReleased est garde pour qu'un bouton enfonce puisse quand meme se relacher
    Input withoutMouse() const {
        Input input = *this;
        input.mouse = {-100000.0f, -100000.0f};
        input.mousePressed = false;
        input.wheel = 0.0f;
        return input;
    }
};

}
