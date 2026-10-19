#include "states/pauseState.hpp"
#include "states/mainMenuState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "raygui.h"

void PauseState::update(App* app) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        app->popState(); 
        return; 
    }
}

void PauseState::render(App* app) {
    DrawText("PAUSE", 1200 / 2 - MeasureText("PAUSE", 60) / 2, 150, 60, RAYWHITE);

    int btnWidth = 200;
    int btnHeight = 50;
    int startX = 1200 / 2 - btnWidth / 2; 


    if (GuiButton(Rectangle{(float)startX, 300, (float)btnWidth, (float)btnHeight}, "REPRENDRE")) {
        app->popState();
    }

    if (GuiButton(Rectangle{(float)startX, 380, (float)btnWidth, (float)btnHeight}, "MENU PRINCIPAL")) {
        app->clearAndPush(new MainMenuState());
    }

    if (GuiButton(Rectangle{(float)startX, 460, (float)btnWidth, (float)btnHeight}, "QUITTER LE JEU")) {
        app->quit();
    }
}