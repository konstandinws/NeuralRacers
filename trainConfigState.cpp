#include "trainConfigState.hpp"
#include "app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "pauseState.hpp"

void TrainConfigState::update(App* app)
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        app->pushState(new PauseState());
    }
}

void TrainConfigState::render(App* app)
{
    DrawText("TRAIN CONFIG", 400, 100, 40, RAYWHITE);

    // ajouter des boutons pour configurer l'entrainement de l'IA
}