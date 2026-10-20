#include "states/pauseState.hpp"
#include "states/mainMenuState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "ui/ui.hpp"

void PauseState::onEnter(App* app) {
    root = ui::column({.spacing = 90},
        ui::label({.text = "PAUSE", .fontSize = 60}),
        ui::column({.spacing = 30},
            ui::button({.text = "RESUME",         .onClick = [app] { app->popState(); }}),
            ui::button({.text = "MAIN MENU",      .onClick = [app] { app->clearAndPush(std::make_unique<MainMenuState>()); }}),
            ui::button({.text = "QUIT GAME",      .onClick = [app] { app->quit(); }})
        )
    );
    ui::placeCentered(*root, app->W / 2.0f, 150);
}

void PauseState::update(App* app) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        app->popState();
        return;
    }

    root->update(ui::Input::poll());
}

void PauseState::render(App* app) {
    (void)app;
    root->draw();
}
