#include "trackEditorState.hpp"
#include "app.hpp"       
#include "raylib.h"
#include "raygui.h"
#include "renderer.hpp"
#include "pauseState.hpp"


void TrackEditorState::update(App* app)
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        app->pushState(new PauseState());
    }
}

void TrackEditorState::render(App* app)
{
    DrawText("TRACK EDITOR", 400, 100, 40, RAYWHITE);

    // ajouter des boutons pour créer des murs, des checkpoints, etc.
    
}