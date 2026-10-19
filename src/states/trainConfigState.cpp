#include "states/trainConfigState.hpp"
#include "core/app.hpp"
#include "ai/aiCar.hpp"
#include "io/fileLoader.hpp"
#include "io/trackSerializer.hpp"
#include "states/mainMenuState.hpp"
#include "states/trainingState.hpp"
#include "ui/ui.hpp"
#include <algorithm>

static constexpr float LIST_WIDTH = 300.0f;
static constexpr float LIST_HEIGHT = 340.0f;
// un peu plus large que la liste pour les messages en dessous
static constexpr float COLUMN_WIDTH = 340.0f;

TrainConfigState::TrainConfigState() {
    loadFilesNameFromFolder(TRACKS_DIR, trackFiles);
    modelNames = listModels();

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

void TrainConfigState::selectModel(int index) {
    selectedModel = index;
    modelError.clear();
    selectedModelData.reset();

    if (index >= 0) {
        selectedModelData = loadModel(modelNames[index]);
        if (!selectedModelData) {
            modelError = "Fichier de modele invalide";
        } else if (selectedModelData->network.getTopology() != AI_TOPOLOGY) {
            // reseau sauvegarde avec une autre architecture : inutilisable par les voitures actuelles
            modelError = "Modele incompatible (autre architecture)";
            selectedModelData.reset();
        }
    }
    root.invalidate();
}

void TrainConfigState::launch(App* app) {
    TrainingConfig config;
    config.trackPath = TRACKS_DIR + trackFiles[selectedTrack];
    config.carCount = carCount;

    if (selectedModel < 0) {
        bool useDefaultName = newModelName.empty();
        config.modelName = useDefaultName ? nextDefaultModelName() : newModelName;
        if (!useDefaultName && modelExists(config.modelName)) {
            modelError = "Un modele \"" + config.modelName + "\" existe deja";
            root.invalidate();
            return;
        }
    } else {
        config.modelName = modelNames[selectedModel];
        config.startModel = selectedModelData;
    }

    app->changeState(new TrainingState(config));
}

ui::ComponentPtr TrainConfigState::selectList(const std::vector<std::string>& labels, int selected, std::function<void(int)> onSelect, float* scroll) {
    ui::ScrollViewProps listProps = {
        .size = {LIST_WIDTH, LIST_HEIGHT},
        .offset = *scroll,
        .onScroll = [scroll](float offset) { *scroll = offset; },
    };
    float buttonWidth = listProps.size.x - 2 * listProps.padding - 2 * ui::ScrollView::SCROLLBAR_WIDTH;

    std::vector<ui::ComponentPtr> buttons;
    for (int i = 0; i < (int)labels.size(); i++) {
        buttons.push_back(ui::button({
            .text = labels[i],
            .onClick = [onSelect, i] { onSelect(i); },
            .size = {buttonWidth, 50},
            .selected = i == selected,
        }));
    }
    return ui::scrollView(listProps, ui::columnOf({.spacing = 10}, std::move(buttons)));
}

ui::ComponentPtr TrainConfigState::buildUI(App* app) {
    // --- circuit ---
    ui::ComponentPtr trackList = trackFiles.empty()
        ? ui::label({.text = "Aucun circuit dans " + TRACKS_DIR, .color = ui::theme::accent})
        : selectList(trackFiles, selectedTrack, [this](int i) { selectTrack(i); }, &trackScroll);

    // sans checkpoints, la fitness retombe sur la distance a vol d'oiseau jusqu'a l'arrivee
    bool noCheckpoints = selectedTrack >= 0 && selectedCheckpoints == 0;
    std::string trackInfo = selectedTrack < 0 ? ""
        : noCheckpoints ? "Pas de checkpoints : l'IA apprendra mal"
        : std::to_string(selectedCheckpoints) + " checkpoints";

    // --- modele : le 1er element de la liste est "nouveau modele" ---
    std::vector<std::string> modelLabels = {"+ Nouveau modele"};
    modelLabels.insert(modelLabels.end(), modelNames.begin(), modelNames.end());
    ui::ComponentPtr modelList = selectList(modelLabels, selectedModel + 1, [this](int i) { selectModel(i - 1); }, &modelScroll);

    bool isNewModel = selectedModel < 0;
    std::string defaultModelName = nextDefaultModelName();

    // champ du nom pour un nouveau modele, sinon rien (colonne vide de meme hauteur pour ne pas faire bouger l'ecran)
    ui::ComponentPtr modelNameInput = isNewModel
        ? ui::textInput({
              .value = newModelName,
              .placeholder = defaultModelName,
              .onChange = [this](const std::string& value) { newModelName = value; },
              .filter = fileNameCharFilter,
              .maxLength = 24,
              .size = {LIST_WIDTH, 44},
          })
        : ui::label({.text = "", .fontSize = 44});

    std::string modelInfo = !modelError.empty() ? modelError
        : isNewModel ? "Laisser vide pour : " + defaultModelName
        : "Deja entraine : " + std::to_string(selectedModelData->generation) + " generations";
    Color modelInfoColor = !modelError.empty() ? ui::theme::accent : ui::theme::border;

    bool canLaunch = selectedTrack >= 0 && (isNewModel || selectedModelData.has_value());

    auto tree = ui::column({.spacing = 40},
        ui::label({.text = "ENTRAINEMENT", .fontSize = 40}),
        ui::row({.spacing = 30, .align = ui::Align::Start},
            ui::column({.spacing = 15, .align = ui::Align::Start, .minWidth = COLUMN_WIDTH},
                ui::label({.text = "Circuit", .fontSize = 24}),
                std::move(trackList),
                ui::label({.text = trackInfo, .fontSize = 18, .color = noCheckpoints ? ORANGE : ui::theme::border})
            ),
            ui::column({.spacing = 15, .align = ui::Align::Start, .minWidth = COLUMN_WIDTH},
                ui::label({.text = "Modele", .fontSize = 24}),
                std::move(modelList),
                std::move(modelNameInput),
                ui::label({.text = modelInfo, .fontSize = 18, .color = modelInfoColor})
            ),
            ui::column({.spacing = 15, .align = ui::Align::Start},
                ui::label({.text = "Nombre de voitures", .fontSize = 24}),
                ui::stepper({
                    .value = carCount,
                    .min = MIN_CARS,
                    .max = MAX_CARS,
                    .steps = {10, 1},
                    .onChange = [this](int value) { carCount = value; },
                }),
                ui::label({
                    .text = "Entre " + std::to_string(MIN_CARS) + " et " + std::to_string(MAX_CARS),
                    .fontSize = 18,
                    .color = ui::theme::border,
                })
            )
        ),
        ui::row({.spacing = 20},
            ui::button({.text = "RETOUR", .onClick = [app] { app->changeState(new MainMenuState()); }}),
            ui::button({.text = "LANCER", .onClick = [this, app] { launch(app); }, .disabled = !canLaunch})
        )
    );
    ui::placeCentered(*tree, app->W / 2.0f, 30);
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
