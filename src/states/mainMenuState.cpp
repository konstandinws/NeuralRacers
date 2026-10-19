#include "states/mainMenuState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "ui/ui.hpp"
#include "states/circuitSelectState.hpp"
#include "states/trackEditorState.hpp"
#include "states/trainConfigState.hpp"

void MainMenuState::onEnter(App* app)
{
    // Si on clique, on dit App de changer d'état
    root = ui::column({.spacing = 160},
        ui::label({.text = "Trackmania", .fontSize = 40}),
        ui::column({.spacing = 50},
            ui::button({.text = "JOUER",     .onClick = [app] { app->changeState(new CircuitSelectState()); }}),
            ui::button({.text = "EDITEUR",   .onClick = [app] { app->changeState(new TrackEditorState()); }}),
            ui::button({.text = "ENTRAINER", .onClick = [app] { app->changeState(new TrainConfigState()); }}),
            ui::button({.text = "QUITTER",   .onClick = [app] { app->quit(); }})
        )
    );
    ui::placeCentered(*root, app->W / 2.0f, 100);
}

void MainMenuState::update(App* app)
{
    (void)app;
    root->update(ui::Input::poll());
}

void MainMenuState::render(App* app)
{
    (void)app;
    root->draw();
}
