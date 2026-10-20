#include "states/mainMenuState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "ui/ui.hpp"
#include "states/circuitSelectState.hpp"
#include "states/trackEditorState.hpp"
#include "states/trainConfigState.hpp"

void MainMenuState::onEnter(App* app)
{
    // on click, ask App to change state
    root = ui::column({.spacing = 160},
        ui::label({.text = "Trackmania", .fontSize = 40}),
        ui::column({.spacing = 50},
            ui::button({.text = "PLAY",      .onClick = [app] { app->changeState(std::make_unique<CircuitSelectState>()); }}),
            ui::button({.text = "EDITOR",    .onClick = [app] { app->changeState(std::make_unique<TrackEditorState>()); }}),
            ui::button({.text = "TRAIN",     .onClick = [app] { app->changeState(std::make_unique<TrainConfigState>()); }}),
            ui::button({.text = "QUIT",      .onClick = [app] { app->quit(); }})
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
