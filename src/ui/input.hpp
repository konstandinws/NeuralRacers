#pragma once
#include "raylib.h"

namespace ui {

// etat de la souris pour une frame, lu une seule fois puis transmis a tout l'arbre de composants
struct Input {
    Vector2 mouse = {0, 0};
    bool mousePressed = false;   // bouton gauche enfonce pendant cette frame
    bool mouseReleased = false;  // bouton gauche relache pendant cette frame
    float wheel = 0.0f;

    static Input poll() {
        return {
            GetMousePosition(),
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
            IsMouseButtonReleased(MOUSE_BUTTON_LEFT),
            GetMouseWheelMove()
        };
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
