#include "trackEditorState.hpp"
#include "app.hpp"
#include "raygui.h"
#include "trackEditor.hpp"

TrackEditorState::TrackEditorState() {
    camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { 0, 0 }; // L'origine de la caméra
    camera.target = { 0, 0 }; // La cible de la caméra
}

void TrackEditorState::update(App* app) {

    if(IsKeyDown(KEY_W)) {
        camera.target.y -= 20; // Déplacer la caméra vers le haut
    }
    if(IsKeyDown(KEY_S)) {
        camera.target.y += 20; // Déplacer la caméra vers le bas
    }
    if(IsKeyDown(KEY_A)) {
        camera.target.x -= 20; // Déplacer la caméra vers la gauche
    }
    if(IsKeyDown(KEY_D)) {
        camera.target.x += 20; // Déplacer la caméra vers la droite
    }

    if (IsKeyPressed(KEY_Q)) {
        camera.zoom *= 1.1f; // Zoomer
    }
    if (IsKeyPressed(KEY_E)) {
        camera.zoom /= 1.1f; // Dézoomer
    }

    if(IsKeyPressed(KEY_SPACE)) {
        editor.clear();
    }

}

void TrackEditorState::render(App* app) {
    BeginMode2D(camera);
    editor.handleClick(camera);
    editor.draw();
    EndMode2D();
}