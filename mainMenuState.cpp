#include "mainMenuState.hpp"
#include "app.hpp"       
#include "raylib.h"
#include "raygui.h"
#include "renderer.hpp"
#include "circuitSelectState.hpp"
#include "trackEditorState.hpp"
#include "pauseState.hpp"
#include "trainConfigState.hpp"

void MainMenuState::update(App* app)
{

}

void MainMenuState::render(App* app)
{
    int startX = 1200 / 2 - 100;

    DrawText("Trackmania", startX - 30, 100, 40, RAYWHITE);

    // Si on clique, on dit App de changer d'état 
    if (GuiButton(Rectangle{(float)startX, 300, 200, 50}, "JOUER")) {
        app->changeState(new CircuitSelectState()); 
    }

    if (GuiButton(Rectangle{(float)startX, 400, 200, 50}, "EDITEUR")) {
        app->changeState(new TrackEditorState());
    }

    if (GuiButton(Rectangle{(float)startX, 500, 200, 50}, "ENTRAINER")) {
        app->changeState(new TrainConfigState());
    }

    if (GuiButton(Rectangle{(float)startX, 600, 200, 50}, "QUITTER")) {
        app->quit();
    }
}