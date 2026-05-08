#include "trackEditorState.hpp"
#include "app.hpp"
#include "raygui.h"
#include "raymath.h"
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

    if(editor.isComplete) {
        if(editor.start == nullptr) {
            editor.placeStart(camera);
        } else if(editor.angleStart == nullptr && editor.start != nullptr) {
            editor.placeAngleStart(camera);
        } else if(editor.finish == nullptr) {
            editor.placeFinish(camera);
        }
    }

}

void TrackEditorState::render(App* app) {
    DrawText("Track Editor - WASD to move, Q/E to zoom, SPACE to clear, G to save", 10, 10, 20, RAYWHITE);
    BeginMode2D(camera);
    editor.handleClick(camera);

    // mettre cela dans le renderer (flemme pour l'instant)
    editor.draw();
    if(editor.angleStart == nullptr && editor.start != nullptr) {
        DrawLine(editor.start->x, editor.start->y, GetScreenToWorld2D(GetMousePosition(), camera).x, GetScreenToWorld2D(GetMousePosition(), camera).y, BLUE);
    }
    if(editor.angleStart != nullptr) {
        DrawLine(editor.start->x, editor.start->y, editor.start->x + cos(*editor.angleStart) * 50, editor.start->y + sin(*editor.angleStart) * 50, BLUE);
    }
    EndMode2D();
}