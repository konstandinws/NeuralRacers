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
    root.mount([this, app] { return buildUI(app); });
}

void CircuitSelectState::play(App* app, const std::string& file)
{
    // load before switching screen: an invalid file shows an error instead of crashing the game
    std::string error;
    std::optional<Track> track = loadTrack(TRACKS_DIR + file, &error);
    if (!track) {
        loadError = file + " : " + error;
        root.invalidate();
        return;
    }
    app->changeState(std::make_unique<PlayState>(std::move(*track)));
}

ui::ComponentPtr CircuitSelectState::buildUI(App* app)
{
    ui::ScrollViewProps listProps = {
        .size = {400, 500},
        .offset = listScroll,
        .onScroll = [this](float offset) { listScroll = offset; },
    };
    float buttonWidth = listProps.size.x - 2 * listProps.padding - 2 * ui::ScrollView::SCROLLBAR_WIDTH;

    // one button per track found in the folder
    std::vector<ui::ComponentPtr> trackButtons;
    for (const std::string& file : trackFiles) {
        trackButtons.push_back(ui::button({
            .text = file,
            .onClick = [this, app, file] { play(app, file); },
            .size = {buttonWidth, 50},
        }));
    }

    auto tree = ui::column({.spacing = 30},
        ui::label({.text = "SELECT YOUR CIRCUIT", .fontSize = 40}),
        ui::scrollView(listProps, ui::columnOf({.spacing = 10}, std::move(trackButtons))),
        ui::label({.text = loadError, .fontSize = 18, .color = ui::theme::accent}),
        ui::button({.text = "BACK", .onClick = [app] { app->changeState(std::make_unique<MainMenuState>()); }})
    );
    ui::placeCentered(*tree, app->W / 2.0f, 40);
    return tree;
}

void CircuitSelectState::update(App* app)
{
    (void)app;
    root.update(ui::Input::poll());
}

void CircuitSelectState::render(App* app)
{
    (void)app;
    root.draw();
}
