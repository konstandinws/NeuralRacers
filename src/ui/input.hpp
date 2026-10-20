#pragma once
#include <vector>
#include "raylib.h"

namespace ui {

// mouse and keyboard state for one frame, read once then passed to the whole component tree
// poll() must be called every frame: it drains raylib's typed character queue
struct Input {
    Vector2 mouse = {0, 0};
    bool mousePressed = false;   // left button pressed this frame
    bool mouseReleased = false;  // left button released this frame
    float wheel = 0.0f;

    std::vector<int> typed;      // characters typed this frame (codepoints)
    bool backspace = false;      // press or repeat (key held down)
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

    // copy without the mouse, for hidden areas (e.g. buttons outside the visible part of a ScrollView)
    // mouseReleased is kept so a pressed button can still be released
    Input withoutMouse() const {
        Input input = *this;
        input.mouse = {-100000.0f, -100000.0f};
        input.mousePressed = false;
        input.wheel = 0.0f;
        return input;
    }
};

}
