#include "circuitSelectState.hpp"
#include "app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "pauseState.hpp"
#include "fileLoader.hpp"


void CircuitSelectState::update(App* app)
{

}

void CircuitSelectState::render(App* app)
{
    DrawText("SELECT YOUR CIRCUIT", 400, 100, 40, RAYWHITE);

    // charger les circuits depuis un dossier et les afficher dynamiquement

    if (GuiButton(Rectangle{400, 300, 200, 50}, "CIRCUIT 1")) {
        // app->changeState(new PlayState());
        loadCircuitsFromFolder("Tracks");
    }

    if (GuiButton(Rectangle{400, 400, 200, 50}, "CIRCUIT 2")) {
        // app->changeState(new PlayState());
    }

    if (GuiButton(Rectangle{400, 500, 200, 50}, "CIRCUIT 3")) {
        // app->changeState(new PlayState());
    }
}   