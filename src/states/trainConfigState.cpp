#include "states/trainConfigState.hpp"
#include "core/app.hpp"
#include "io/fileLoader.hpp"
#include "io/trackSerializer.hpp"
#include "states/mainMenuState.hpp"
#include "states/trainingState.hpp"
#include "ui/ui.hpp"
#include <algorithm>

TrainConfigState::TrainConfigState() {
    loadFilesNameFromFolder(TRACKS_DIR, trackFiles);

    // circuit d'entrainement par defaut s'il existe, sinon le premier
    auto it = std::find(trackFiles.begin(), trackFiles.end(), "trackTest.txt");
    if (it != trackFiles.end()) {
        selectTrack((int)(it - trackFiles.begin()));
    } else if (!trackFiles.empty()) {
        selectTrack(0);
    }
}

void TrainConfigState::onEnter(App* app) {
    root.mount([this, app] { return buildUI(app); });
}

void TrainConfigState::selectTrack(int index) {
    selectedTrack = index;
    // on charge le circuit pour prevenir s'il n'a pas de checkpoints
    selectedCheckpoints = (int)loadTrack(TRACKS_DIR + trackFiles[index]).checkpoints.size();
    root.invalidate();
}

ui::ComponentPtr TrainConfigState::buildUI(App* app) {
    // liste des circuits, le circuit choisi est mis en evidence
    ui::ScrollViewProps listProps = {
        .size = {400, 420},
        .offset = listScroll,
        .onScroll = [this](float offset) { listScroll = offset; },
    };
    float buttonWidth = listProps.size.x - 2 * listProps.padding - 2 * ui::ScrollView::SCROLLBAR_WIDTH;

    std::vector<ui::ComponentPtr> trackButtons;
    for (int i = 0; i < (int)trackFiles.size(); i++) {
        trackButtons.push_back(ui::button({
            .text = trackFiles[i],
            .onClick = [this, i] { selectTrack(i); },
            .size = {buttonWidth, 50},
            .selected = i == selectedTrack,
        }));
    }

    ui::ComponentPtr trackList = trackFiles.empty()
        ? ui::label({.text = "Aucun circuit dans " + TRACKS_DIR, .color = ui::theme::accent})
        : ui::scrollView(listProps, ui::columnOf({.spacing = 10}, std::move(trackButtons)));

    // sans checkpoints, la fitness retombe sur la distance a vol d'oiseau jusqu'a l'arrivee
    bool noCheckpoints = selectedTrack >= 0 && selectedCheckpoints == 0;
    std::string trackInfo = selectedTrack < 0 ? ""
        : noCheckpoints ? "Pas de checkpoints : l'IA apprendra mal"
        : std::to_string(selectedCheckpoints) + " checkpoints";

    auto tree = ui::column({.spacing = 40},
        ui::label({.text = "ENTRAINEMENT", .fontSize = 40}),
        ui::row({.spacing = 80, .align = ui::Align::Start},
            ui::column({.spacing = 15, .align = ui::Align::Start},
                ui::label({.text = "Circuit", .fontSize = 24}),
                std::move(trackList),
                ui::label({.text = trackInfo, .fontSize = 18, .color = noCheckpoints ? ORANGE : ui::theme::border})
            ),
            ui::column({.spacing = 15, .align = ui::Align::Start},
                ui::label({.text = "Nombre de voitures", .fontSize = 24}),
                ui::stepper({
                    .value = carCount,
                    .min = MIN_CARS,
                    .max = MAX_CARS,
                    .step = CAR_STEP,
                    .onChange = [this](int value) { carCount = value; },
                })
            )
        ),
        ui::row({.spacing = 20},
            ui::button({.text = "RETOUR", .onClick = [app] { app->changeState(new MainMenuState()); }}),
            ui::button({
                .text = "LANCER",
                .onClick = [this, app] {
                    app->changeState(new TrainingState(TRACKS_DIR + trackFiles[selectedTrack], carCount));
                },
                .disabled = selectedTrack < 0,
            })
        )
    );
    ui::placeCentered(*tree, app->W / 2.0f, 40);
    return tree;
}

void TrainConfigState::update(App* app) {
    (void)app;
    root.update(ui::Input::poll());
}

void TrainConfigState::render(App* app) {
    (void)app;
    root.draw();
}
