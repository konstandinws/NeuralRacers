#include "trackEditorState.hpp"
#include "app.hpp"
#include "raygui.h"
#include "raymath.h"
#include "trackEditor.hpp"
#include "trackSerializer.hpp"
#include "track.hpp"

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

    if (IsKeyPressed(KEY_G)) {
        if(editor.isFinished()) {
            Track track = editor.saveTrack();
            saveTrack(track);
        }
    }

    if(editor.isComplete) {
        if(editor.start == nullptr) {
            editor.placeStart(camera);
        } else if(editor.angleStart == nullptr && editor.start != nullptr) {
            editor.placeAngleStart(camera);
        } else if(editor.finish == nullptr) {
            editor.placeFinish(camera);
        } else {
            editor.placeCheckpoint(camera);
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        editor.removeLastCheckpoint();
    }

}

void TrackEditorState::render(App* app) {
    DrawText("Track Editor - WASD to move, Q/E to zoom, SPACE to clear, G to save", 10, 10, 20, RAYWHITE);
    if (editor.finish != nullptr) {
        DrawText("Checkpoints: 2 clicks each, in driving order - BACKSPACE to undo", 10, 35, 20, SKYBLUE);
    }
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
    if(editor.pendingCheckpoint) {
        Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
        DrawLine(editor.pendingCheckpoint->x, editor.pendingCheckpoint->y, mouse.x, mouse.y, SKYBLUE);
    }
    EndMode2D();
}