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
// a bit wider than the list for the messages below it
static constexpr float COLUMN_WIDTH = 340.0f;

TrainConfigState::TrainConfigState() {
    loadFilesNameFromFolder(TRACKS_DIR, trackFiles);
    modelNames = listModels();

    // default training track if it exists, otherwise the first one
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
    // loaded now: to warn about missing checkpoints, refuse invalid files, and hand it to the training
    trackError.clear();
    selectedTrackData = loadTrack(TRACKS_DIR + trackFiles[index], &trackError);
    root.invalidate();
}

void TrainConfigState::selectModel(int index) {
    selectedModel = index;
    modelError.clear();
    selectedModelData.reset();

    selectedModelAdapted = false;

    if (index >= 0) {
        selectedModelData = loadModel(modelNames[index]);
        if (!selectedModelData) {
            modelError = "Invalid model file";
            root.invalidate();
            return;
        }

        // old model without the speed inputs: add them with zero weights
        // (it drives exactly as before, then learns to use them)
        std::vector<int> topology = selectedModelData->network.getTopology();
        bool onlyFewerInputs = topology.size() == AI_TOPOLOGY.size() && topology[0] < AI_TOPOLOGY[0]
            && std::equal(topology.begin() + 1, topology.end(), AI_TOPOLOGY.begin() + 1);
        if (onlyFewerInputs) {
            selectedModelAdapted = selectedModelData->network.widenInputs(AI_TOPOLOGY[0]);
        }

        if (selectedModelData->network.getTopology() != AI_TOPOLOGY) {
            // network saved with another architecture: unusable by the current cars
            modelError = "Incompatible model (different architecture)";
            selectedModelData.reset();
        }
    }
    root.invalidate();
}

void TrainConfigState::launch(App* app) {
    TrainingConfig config;
    config.track = *selectedTrackData;
    config.carCount = carCount;

    if (selectedModel < 0) {
        bool useDefaultName = newModelName.empty();
        config.modelName = useDefaultName ? nextDefaultModelName() : newModelName;
        if (!useDefaultName && modelExists(config.modelName)) {
            modelError = "A model named \"" + config.modelName + "\" already exists";
            root.invalidate();
            return;
        }
    } else {
        config.modelName = modelNames[selectedModel];
        config.startModel = selectedModelData;
    }

    app->changeState(std::make_unique<TrainingState>(config));
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
    // --- track ---
    ui::ComponentPtr trackList = trackFiles.empty()
        ? ui::label({.text = "No tracks in " + TRACKS_DIR, .color = ui::theme::accent})
        : selectList(trackFiles, selectedTrack, [this](int i) { selectTrack(i); }, &trackScroll);

    // without checkpoints, fitness falls back to the straight-line distance to the finish
    int checkpointCount = selectedTrackData ? (int)selectedTrackData->checkpoints.size() : 0;
    bool trackInvalid = selectedTrack >= 0 && !selectedTrackData;
    bool noCheckpoints = selectedTrackData && checkpointCount == 0;
    std::string trackInfo = selectedTrack < 0 ? ""
        : trackInvalid ? trackError
        : noCheckpoints ? "No checkpoints: the AI will learn poorly"
        : std::to_string(checkpointCount) + " checkpoints";
    Color trackInfoColor = trackInvalid ? ui::theme::accent : noCheckpoints ? ORANGE : ui::theme::border;

    // --- model: the 1st item in the list is "new model" ---
    std::vector<std::string> modelLabels = {"+ New model"};
    modelLabels.insert(modelLabels.end(), modelNames.begin(), modelNames.end());
    ui::ComponentPtr modelList = selectList(modelLabels, selectedModel + 1, [this](int i) { selectModel(i - 1); }, &modelScroll);

    bool isNewModel = selectedModel < 0;
    std::string defaultModelName = nextDefaultModelName();

    // name field for a new model, otherwise nothing (empty column of the same height so the screen does not shift)
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
        : isNewModel ? "Leave empty for: " + defaultModelName
        : selectedModelAdapted ? std::to_string(selectedModelData->generation) + " generations, adapted to speed inputs"
        : "Already trained: " + std::to_string(selectedModelData->generation) + " generations";
    Color modelInfoColor = !modelError.empty() ? ui::theme::accent : ui::theme::border;

    bool canLaunch = selectedTrackData.has_value() && (isNewModel || selectedModelData.has_value());

    auto tree = ui::column({.spacing = 40},
        ui::label({.text = "TRAINING", .fontSize = 40}),
        ui::row({.spacing = 30, .align = ui::Align::Start},
            ui::column({.spacing = 15, .align = ui::Align::Start, .minWidth = COLUMN_WIDTH},
                ui::label({.text = "Track", .fontSize = 24}),
                std::move(trackList),
                ui::label({.text = trackInfo, .fontSize = 18, .color = trackInfoColor})
            ),
            ui::column({.spacing = 15, .align = ui::Align::Start, .minWidth = COLUMN_WIDTH},
                ui::label({.text = "Model", .fontSize = 24}),
                std::move(modelList),
                std::move(modelNameInput),
                ui::label({.text = modelInfo, .fontSize = 18, .color = modelInfoColor})
            ),
            ui::column({.spacing = 15, .align = ui::Align::Start},
                ui::label({.text = "Number of cars", .fontSize = 24}),
                ui::stepper({
                    .value = carCount,
                    .min = MIN_CARS,
                    .max = MAX_CARS,
                    .steps = {10, 1},
                    .onChange = [this](int value) { carCount = value; },
                }),
                ui::label({
                    .text = "Between " + std::to_string(MIN_CARS) + " and " + std::to_string(MAX_CARS),
                    .fontSize = 18,
                    .color = ui::theme::border,
                })
            )
        ),
        ui::row({.spacing = 20},
            ui::button({.text = "BACK", .onClick = [app] { app->changeState(std::make_unique<MainMenuState>()); }}),
            ui::button({.text = "START", .onClick = [this, app] { launch(app); }, .disabled = !canLaunch})
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
