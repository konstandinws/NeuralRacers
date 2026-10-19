#include "states/circuitSelectState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "ui/ui.hpp"
#include "io/fileLoader.hpp"
#include "states/mainMenuState.hpp"
#include "states/playState.hpp"
#include "io/trackSerializer.hpp"

CircuitSelectState::CircuitSelectState() {
    loadFilesNameFromFolder(TRACKS_DIR, trackFiles);
}

void CircuitSelectState::onEnter(App* app)
{
    ui::ScrollViewProps listProps = {.size = {400, 500}};
    float buttonWidth = listProps.size.x - 2 * listProps.padding - 2 * ui::ScrollView::SCROLLBAR_WIDTH;

    // un bouton par circuit trouve dans le dossier
    std::vector<ui::ComponentPtr> trackButtons;
    for (const std::string& file : trackFiles) {
        trackButtons.push_back(ui::button({
            .text = file,
            .onClick = [app, file] { app->changeState(new PlayState(TRACKS_DIR + file)); },
            .size = {buttonWidth, 50},
        }));
    }

    root = ui::column({.spacing = 40},
        ui::label({.text = "SELECT YOUR CIRCUIT", .fontSize = 40}),
        ui::scrollView(listProps, ui::columnOf({.spacing = 10}, std::move(trackButtons))),
        ui::button({.text = "RETOUR", .onClick = [app] { app->changeState(new MainMenuState()); }})
    );
    ui::placeCentered(*root, app->W / 2.0f, 50);
}

void CircuitSelectState::update(App* app)
{
    (void)app;
    root->update(ui::Input::poll());
}

void CircuitSelectState::render(App* app)
{
    (void)app;
    root->draw();
}
